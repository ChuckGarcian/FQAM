
## 11.23.24 Saturday

Current:

- Added 'assertf.h' for better assert statements
- Implemented temporary generators for pauli x, z and I. 

Next:

- Goal is to get staging logic to work.
- Be able to stage new operators to list.
- Then start execution or render.

## 11.25.24 Monday

Completed:  

- Update pauli module with better function names

Next:

- Add statevector computation
- Change the way pauli operator matrices are generated so they include dimension size

- I was working on Steeb and Hardy, and I started to formalize how one can derive a unitary operator given an operator specification. Specifically, I was trying an example of deriving U_not given the NOT operation. I need to finish number 4.

Crux: Deriving a matrix representation is easy given we have the unitary operator representation. The hard part is transitioning from input to output in the following

**Input:** NOT operator description:

$$
NOT (\ket{\psi}) = 
\begin{cases}
\ket{0} \mapsto \ket{1} \\
\ket{1} \mapsto \ket{0} \\
\end{cases}
$$

**Output:** Unitary operator of NOT

$$
U = \ket{0}\bra{1} + \ket{1}\bra{0}
$$

$$
\{Q: \ket{\psi} = \ket{\psi}\}
$$

## 11.28.24 Thursday

- Get visual working

## 12.03.24 Tuesday

- Rewrite pauli module such that it uses the new operator subsystem/api

## 12.05.24 Thursday

- Libflame has a special matrix type for hermitian which only stores either the lower or upper triangular. I should look into using that at some point