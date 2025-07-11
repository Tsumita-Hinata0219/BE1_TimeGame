#include "Object3d.hlsli"


// マテリアル
struct Material
{
    float4 color;
    float4x4 uvTransform;
    uint textureIndex;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);

// カメラ
struct ViewProjectionMatrix
{
    float4x4 view;
    float4x4 projection;
    float4x4 orthoGraphic;
    float3 cameraPosition;
};
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMat : register(b1);

// ライト
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
    float shininess;
    int eneble;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);

// 環境マップ
struct Environment
{
    float scale;
    int enable;
};
ConstantBuffer<Environment> gEnvironment : register(b3);
TextureCube<float4> gEnvironmentTexture : register(t1);

// 色加算
struct ColorAddition
{
    float4 addColor;
    float intensity;
    int enable;
};
ConstantBuffer<ColorAddition> gColorAddition : register(b4);



SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    // UV変換を適用
    float4 transformedUV = mul(float4(input.texCoord, 0.0f, 1.0f), gMaterial.uvTransform);

    // テクスチャから色をサンプリング
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    // アルファ値が0.9f未満なら描画をスキップ
    if (textureColor.a == 0.0f)
    {
        discard;
    }
    
    // ライト処理
    if (gDirectionalLight.eneble) // ライトが有効な場合
    {
        // 法線ベクトルの計算
        float3 normal = normalize(input.normal);
        
        // ライトの方向ベクトルを正規化
        float3 lightDir = normalize(gDirectionalLight.direction);
        
        // 視線ベクトルの計算
        float3 toEye = normalize(gViewProjectionMat.cameraPosition - input.worldPos);
        
        // リフレクトベクトルの計算
        float3 reflectLight = reflect(lightDir, normal);
        
        // ハーフベクトルの計算
        float3 halfVector = normalize(lightDir + toEye);
        
        // 拡散反射の計算
        float NdotL = dot(normal, lightDir);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        // 鏡面反射の計算
        float NdotH = dot(normal, halfVector);
        float3 specularPow = pow(saturate(NdotH), gDirectionalLight.shininess);
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
       
        // 拡散反射と鏡面反射の合成
        float3 finalColor = diffuse;
        
        // テクスチャのアルファが0の場合はピクセルを破棄
        if (textureColor.a <= 0.5f)
        {
            discard;
        }
        
        // 色の設定
        output.color.rgb = finalColor;
    }
    else
    {
        // ライティングが無効な場合は、テクスチャの色にマテリアルの色を掛け合わせる
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
    }
    
    // 環境マップ
    if (gEnvironment.enable)
    {
        float3 cameraToPos = normalize(input.worldPos - gViewProjectionMat.cameraPosition);
        float3 reflectedVector = reflect(cameraToPos, normalize(input.normal));
        float4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
        environmentColor *= gEnvironment.scale;
        output.color.rgb += environmentColor.rgb;
    }
    
    // 色加算
    if (gColorAddition.enable)
    {
        output.color.rgb += gColorAddition.addColor.rgb * gColorAddition.intensity;
    }

    // アルファ値の設定
    output.color.a = gMaterial.color.a * textureColor.a;

    return output;
}