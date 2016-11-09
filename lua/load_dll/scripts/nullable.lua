local nullable_metatable = {

	__index  = function (t, k)
		print("__index:", t, k)
		local private = rawget(t, "__private__")
		return private.k
	end,

	__newindex  = function (t, k, v)
		print("__newindex:", t, k, v)
		local private = rawget(t, "__private__")
		private.k = v
		if v == nil then	
			print("__newindex: store nil")
			local null = rawget(t, "__null__")
			null.k = true
		end
	end

}

local test = setmetatable({__private__ = {}, __null__ = {}}, nullable_metatable)

test.a = 10
print("A:", table.a)
--test.a = nil
