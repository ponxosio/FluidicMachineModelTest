import abc

class Mixer(object):
	__metaclass__  = abc.ABCMeta

	@abc.abstractmethod
	def __init__(self, params):
		"""constructor"""

	@abc.abstractmethod
	def mix(self, communications, intensity):
		"""must send instructions to stir the liquid at the given real number "intensity" Hz, 
		communication with the machine must be done via communications object
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""
	
	@abc.abstractmethod
	def stopMixing(self, communications):
		"""must send instructions to stop the actuator, 
		communication with the machine must be done via communications object
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""