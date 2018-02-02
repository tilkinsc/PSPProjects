
local key = {
	SELECT	= 0x000001,
	START	= 0x000008,
	UP		= 0x000010,
	RIGHT	= 0x000020,
	DOWN	= 0x000040,
	LEFT	= 0x000080,
	LTRIGGER= 0x000100,
	RTRIGGER= 0x000200,
	TRIANGLE= 0x001000,
	CIRCLE	= 0x002000,
	CROSS	= 0x004000,
	SQUARE	= 0x008000
}

while (isRunning()) do
	vblank()
	setDebugScreen(0, 0)
	
	pollPad()
	pollLatch()
	
	print("Hello world!")
	print(_VERSION)
	
	if(isKeyDown(key.CIRCLE)) then
		print("circle is down")
	end
	
end




