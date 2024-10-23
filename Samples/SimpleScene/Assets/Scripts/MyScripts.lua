local ecs = require "ecs"

local function getVel(m1, m2, v1, v2)
    return ((m1-m2)*v1 + 2*m2*v2) / (m1+m2), ((m2-m1)*v2 + 2*m1*v1) / (m1+m2)
end

local function checkCollision(pos1, pos2, collision1, collision2)
    return  collision1.collision_on  and  collision2.collision_on  and
            (math.abs(pos1.x - pos2.x) < collision1.x + collision2.x) and 
            (math.abs(pos1.y - pos2.y) < collision1.y + collision2.y) and 
            (math.abs(pos1.z - pos2.z) < collision1.z + collision2.z)
end

local function timer(it)
    for t, pos, ent in ecs.each(it) do
        if t.timer_on then
            if t.timer < t.max_time then
			   t.timer = t.timer + it.delta_time
		    else
               t.timer_on = false
               pos.x = 1e4 --moves an object instead of deleting it
               --ecs.delete(ent)
            end
        end
    end
end

local function collision_detection(it)
    for pos1, vel1, collision1, ent1 in ecs.each(it) do
        for pos2, vel2, collision2, ent2 in ecs.each(it) do

            if  ecs.get_alive(ent1) < ecs.get_alive(ent2) and
                checkCollision(pos1, pos2, collision1, collision2) then

                local m1 = collision1.x * collision1.y * collision1.z
                local m2 = collision2.x * collision2.y * collision2.z

                vel1.x, vel2.x = getVel(m1, m2, vel1.x, vel2.x)
                vel1.y, vel2.y = getVel(m1, m2, vel1.y, vel2.y)
                vel1.z, vel2.z = getVel(m1, m2, vel1.z, vel2.z)

            end
        end
    end
end

ecs.system(timer, "timer", ecs.OnUpdate, "Timer, Position")
ecs.system(collision_detection, "collision_detection", ecs.OnUpdate, "Position, Velocity, CollisionSize")