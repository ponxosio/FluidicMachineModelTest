import abc

class FluorescenceSensor(object):
	__metaclass__  = abc.ABCMeta

	@abc.abstractmethod
	def __init__(self, params):
		"""constructor"""

	@abc.abstractmethod
	def startMeasuring(self, communications, measurementFrequency, emission, excitation):
		"""must send a command to start reading the Fluorescence, taking samples at "measuremntFrequency" Hz and 
		appliying a ligth of "emission" nm and "excitation" nm.
		
		communication with the machine must be done via communications object.
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""
	
	@abc.abstractmethod
	def getMeasurement(self, communications):
		"""must return the fluorecence read by the sensor in cd
		
		communication with the machine must be done via communications object.
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""