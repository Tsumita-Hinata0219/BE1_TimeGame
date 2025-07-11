return {
    waves = {
        {
            time = 0,
            enemies = {
                { type = "EnemyTypeA", pos = {x=3, y=4}, hp = 10, speed = 2 },
                { type = "EnemyTypeB", pos = {x=6, y=4}, hp = 20, speed = 1.5 }
            },
            blocks = {
                { type = "Wall", pos = {x=2, y=2} },
                { type = "Switch", pos = {x=5, y=2}, trigger = "wave2_start" }
            }
        },
        {
            trigger = "wave2_start",
            enemies = {
                { type = "EnemyTypeA", pos = {x=3, y=1}, hp = 15 },
                { type = "EnemyTypeBoss", pos = {x=5, y=0}, hp = 100 }
            },
            blocks = {
                { type = "Wall", pos = {x=0, y=0} }
            }
        }
    }
}
