import abc

class RoutingValve(object):
	__metaclass__  = abc.ABCMeta
	
	@abc.abstractmethod
	def __init__(self, params):
		"""constructor"""

	@abc.abstractmethod
	def moveToPosition(self, communications, position):
		"""
			must send instructions to the machine to move this valve to the position "position",
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""
	@abc.abstractmethod
	def closeValve(self, communications):
		"""
			must send instructions to the machine to close this valve to position "position",
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""