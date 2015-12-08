-- 읽혀야 하는 모든 Lua script 들은 여기에 기입된다. 
--
package.path = package.path .. ";./lua_script/?.lua"

require "util_function"		-- 가장 먼저 포함되어야 하는 유틸 함수

require "state"

