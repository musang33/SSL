-- ��� ȣ���� �̿��Ͽ� ���µ��� ����
-- ��ũ��Ʈ �� �� ���� ���� ������ �Ѵ�

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
