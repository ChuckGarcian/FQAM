import numpy as np

eye = np.eye (2)
outer00 = np.array ([[1, 0], [0, 0]])

eyeouter = np.kron (outer00, eye)
 
print (eye)
print (outer00)
print (eyeouter)

