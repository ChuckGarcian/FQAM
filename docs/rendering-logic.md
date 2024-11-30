
## Logic behind rendering each time step

Let $i$ denote the current time step and $i+1$ denote the next time step. With every new time step we transition the current $i$ statevector to the $i+1$ statevector by applying the $i$ transition operators. For the many paths rendering feature we want to visualize the probability associated with each state transition. 

To do this we take the current statevector and compute the density matrix by computing the outer product $\ket\psi\bra\psi$ where $\ket\psi$ is the current statevector. The entries $\ket\psi\bra\psi$ tells us.