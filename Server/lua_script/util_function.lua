-- 재귀 호출을 이용하여 상태들을 복사
-- 스크립트 들 중 가장 먼저 읽혀야 한다

function DeepCopy( orig )
	local orig_type = type( orig )
	local copy

	if orig_type == 'table' then
		copy = {}
		for orig_key, orig_value in next, orig, nil do
			copy[DeepCopy(orig_key)] = DeepCopy(orig_value)
		end

		setmetatable(copy, DeepCopy(getmetatable(orig)) )
	else
		copy = orig	
	end

	return copy
end
