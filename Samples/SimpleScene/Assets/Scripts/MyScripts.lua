local ecs = require "ecs"

local function timer(it)
    for t,  v, ent in ecs.each(it) do
        if t.timer < t.max_time then
			t.timer = t.timer + it.delta_time
		else
            t.timer = 0.0
            v.is_visible = false
        end
    end
end

ecs.system(timer, "timer", ecs.OnUpdate, "Timer, Visibility")


