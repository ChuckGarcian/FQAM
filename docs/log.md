
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

Crux: Deriving a matrix representation is easy given we have the unitary operator representation.

**Input:** Not operator description:

$$
NOT (\ket\psi) = 
\begin{cases}
\ket0 \mapsto \ket1 \\
\ket1 \mapsto \ket0 \\
\end{cases}
$$

**Output:** Unitary operator of Not 

$$
U = \ket0\bra1 + \ket1\bra0
$$