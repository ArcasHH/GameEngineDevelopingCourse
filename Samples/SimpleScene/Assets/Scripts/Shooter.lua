local ecs = require "ecs"

local function timer(it)
    for t, v, ent in ecs.each(it) do
        if t.timer < t.max_time then
			t.timer = t.timer + it.delta_time
		else
            t.timer = 0.0
            v.is_visible = false
        end
    end
end

local function collision_detection(it)
    for pos1, vel1, c1, size1, f1, ent1 in ecs.each(it) do
		for pos2, vel2, c2, size2, f2, ent2 in ecs.each(it) do
 
			if  ecs.get_alive(ent1) < ecs.get_alive(ent2) and
                (math.abs(pos1.x - pos2.x) < size1.x + size2.x) and 
                (math.abs(pos1.y - pos2.y) < size1.y + size2.y) and 
                (math.abs(pos1.z - pos2.z) < size1.z + size2.z) then

                c1.value = true
                c2.value = true
                
                f1.x, f2.x = vel2.x, vel1.x
                f1.y, f2.y = vel2.y, vel1.y
                f1.z, f2.z = vel2.z, vel1.z
            end
		end
	end
end

local function collide(it)
    for is_collide, vel, f, ent in ecs.each(it) do
		if is_collide.value then
            is_collide.value = false
            vel.x = f.x
            vel.y = f.y
            vel.z = f.z
        end
	end
end


ecs.system(timer, "timer", ecs.OnUpdate, "Timer, Visibility")
ecs.system(collision_detection, "collision_detection", ecs.OnUpdate, "Position, Velocity, IsCollide, CollisionSize, ApplyForce")
ecs.system(collide, "collide", ecs.OnUpdate, "IsCollide, Velocity, ApplyForce")
