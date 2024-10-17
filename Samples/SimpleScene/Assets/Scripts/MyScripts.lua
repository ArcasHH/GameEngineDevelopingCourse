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
    local pos, vel, size = ecs.columns(it)
	for i = 1, it.count - 1 do
		for j = i + 1, it.count do
            if  (math.abs(pos[i].x - pos[j].x) < size[i].x + size[j].x) and 
                (math.abs(pos[i].y - pos[j].y) < size[i].y + size[j].y) and 
                (math.abs(pos[i].z - pos[j].z) < size[i].z + size[j].z) then
                vel[i].x, vel[j].x = vel[j].x, vel[i].x
                vel[i].y, vel[j].y = vel[j].y, vel[i].y
                vel[i].z, vel[j].z = vel[j].z, vel[i].z
            end
            
		end
	end
end

ecs.system(timer, "timer", ecs.OnUpdate, "Timer, Visibility")
ecs.system(collision_detection, "collision_detection", ecs.OnUpdate, "Position, Velocity, CollisionSize")

