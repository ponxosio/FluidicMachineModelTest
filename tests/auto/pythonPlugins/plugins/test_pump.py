from peristalticPump import PeristalticPump

class TestPump(PeristalticPump):
	
	def __init__(self, params):
		"""constructor"""

	
	def startPumping(self, communications, direction, rate):
		"""
			must send instructions to the machine to start pumping in direction "direction" with a rate of "rate" l/s
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""
		communications.sendString("PUMP " + str(rate) + str(direction));
	
	def stopPump(self, communications):
		"""
			must send instructions to the machine to stop this pump,
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""
		communications.sendString("SPUMP");