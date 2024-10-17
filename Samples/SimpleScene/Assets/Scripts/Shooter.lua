local ecs = require "ecs"

local function getVel(m1, m2, v1, v2)
    return ((m1-m2)*v1 + 2*m2*v2) / (m1+m2), ((m2-m1)*v2 + 2*m1*v1) / (m1+m2)
end

local function timer(it)
    for t, v, ent in ecs.each(it) do
        if t.timer_on then
            if t.timer < t.max_time then
			    t.timer = t.timer + it.delta_time
		    else
                t.timer = 0.0
                v.is_visible = false
            end
        end
    end
end

local function collision_detection(it)
    local pos, vel, size, t = ecs.columns(it)
	for i = 1, it.count - 1 do
		for j = i + 1, it.count do
            if  (math.abs(pos[i].x - pos[j].x) < size[i].x + size[j].x) and 
                (math.abs(pos[i].y - pos[j].y) < size[i].y + size[j].y) and 
                (math.abs(pos[i].z - pos[j].z) < size[i].z + size[j].z) then
                vel[i].x, vel[j].x = vel[j].x, vel[i].x
                vel[i].y, vel[j].y = vel[j].y, vel[i].y
                vel[i].z, vel[j].z = vel[j].z, vel[i].z
                --doesnt work
                t[i].timer_on, t[j].timer_on = true, true
                t[i].max_time, t[j].max_time = 1.0, 4.0
            end
		end
	end
end

ecs.system(timer, "timer", ecs.OnUpdate, "Timer, Visibility")
ecs.system(collision_detection, "collision_detection", ecs.OnUpdate, "Position, Velocity, CollisionSize, Timer")