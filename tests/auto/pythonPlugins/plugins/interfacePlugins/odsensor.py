import abc

class OdSensor(object):
	__metaclass__  = abc.ABCMeta

	@abc.abstractmethod
	def __init__(self, params):
		"""constructor"""

	@abc.abstractmethod
	def startMeasuring(self, communications, measurementFrequency, wavelength):
		"""must send a command to start reading the optical density sensor, taking samples at "measuremntFrequency" Hz and 
		appliying a ligth of "wavelength" nm.
		
		communication with the machine must be done via communications object.
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""
	
	@abc.abstractmethod
	def getMeasurement(self, communications):
		"""must return a real number with the optical density read by the sensor.
		
		communication with the machine must be done via communications object.
		ommunications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synch() -- synchronize with the machine, not always necesary, only for protocols compatibles;
		"""