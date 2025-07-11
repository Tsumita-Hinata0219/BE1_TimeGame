maplayout = {
"##########",
  "#P......E#",
  "#........#",
  "#....G...#",
  "##########"
}

// オブジェクト生成
auto objects = script.GetTable("objects");
for (int i = 0; i < objects.size(); ++i) {
    std::string type = objects[i]["type"];
    auto pos = objects[i]["position"];
    int x = pos["x"];
    int y = pos["y"];

    // 自作のFactoryやManagerで生成
    objectFactory->Create(type, Vector2(x, y));
}