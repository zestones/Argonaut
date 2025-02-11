
# Virtual Machine

Pour affecter correctement une valeur à un tableau dans votre pile d'exécution, vous devez calculer l'adresse exacte de l'emplacement où la valeur doit être stockée. Cette adresse est déterminée par les indices donnés lors de l'accès au tableau et les informations de dimensionnalité (comme les bornes des dimensions) dans la table des déclarations et la table de représentation. Voici comment procéder :

---

## **Mathematical Formulas for Address Computation**

### **1. Address Calculation for Multi-Dimensional Arrays**

To compute the address of an element in a multi-dimensional array $A$:

$$
\text{Address}(A[I_1, I_2, \ldots, I_n]) = B + \sum_{i=1}^n \left( (I_i - L_i) \cdot S_i \right)
$$

Where:

- $B$: Base address of the array.
- $L_i$: Lower bound of the $i^{\text{th}}$ dimension.
- $I_i$: Index in the $i^{\text{th}}$ dimension.
- $S_i = \prod_{j=i+1}^n (U_j - L_j + 1)$: Stride of the $i^{\text{th}}$ dimension, with $S_n = 1$.
- $U_i$: Upper bound of the $i^{\text{th}}$ dimension.

---

#### **2. Address Calculation for Struct Fields**

For a field $f_k$ in a structure $S$:

$$
\text{Address}(S.f_k) = B + O_{f_k}
$$

Where:

- $B$: Base address of the structure.
- $O_{f_k}$: Offset of field $f_k$, calculated as:

$$
O_{f_k} = \sum_{i=1}^{k-1} \text{Size of Field } f_i
$$

---

#### **3. Memory Size Calculations**

- **Array**:

$$
\text{Size of Array} = \prod_{i=1}^n (U_i - L_i + 1) \times \text{Element Size}
$$

- **Structure**:

$$
\text{Size of Struct} = \sum_{i=1}^N \text{Size of Field } f_i + \text{Alignment Padding}
$$

---

#### **4. Combined Address Calculations**

- **Nested Arrays in Structs**:

$$
\text{Address}(S.A[I_1, I_2, \ldots, I_n]) = B + O_A + \sum_{i=1}^n \left( (I_i - L_i) \cdot S_i \right)
$$

Where $O_A$: Offset of array $A$ within the structure $S$.

- **Structs in Arrays**:

$$
\text{Address}(A[I].f_k) = B + (I \cdot T_S) + O_{f_k}
$$

Where $T_S$: Total size of the struct, $O_{f_k}$: Offset of field $f_k$ within the struct.

---

### **Examples**

#### Example 1: Multi-Dimensional Array

- Array $A[3][4]$ with:
  - $B = 1000$, $L_1 = 0, U_1 = 2$, $L_2 = 0, U_2 = 3$.
  - Stride $S_1 = 4$, $S_2 = 1$.

Address of $A[1][2]$ :

$$
\text{Address} = 1000 + (1 \cdot 4) + (2 \cdot 1) = 1006
$$

---

#### Example 2: Structs in Arrays

- Array of structs $S[5]$:
  - Struct size $T_S = 12$, field $f$ offset $O_f = 4$, base $B = 2000$.

Address of $S[3].f$:

$$
\text{Address} = 2000 + (3 \cdot 12) + 4 = 2040
$$

### Notes

- **Dynamic**: Refers to the function frame index of the caller.  
- **Static**: Refers to the frame index of the parent.
