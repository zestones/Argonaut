# Address Computation and Memory Allocation in the Argonaut VM

Understanding how memory allocation and address computation work is essential when implementing a virtual machine (VM) or a compiler for a programming language. This document provides an overview of how the Argonaut VM handles memory allocation in stack frames and computes addresses for variables, arrays, and structures. It includes mathematical formulas for address computation and code implementations that illustrate these concepts.

---

## Memory Allocation in Stack Frames

When a variable is declared within a function or a procedure, the VM needs to allocate memory for it within the current stack frame. The amount of memory allocated depends on the type of the variable, which could be a base type (e.g., `int`, `float`), an array, or a structure. The VM must also handle nested arrays and structures appropriately.

### Variable Declaration and Allocation

The process of declaring and allocating memory for a variable involves several steps:

1. **Identify the Type of Variable**: Determine whether the variable is a base type, an array, or a structure.
2. **Allocate Memory Based on Type**:
   - **Base Types**: Allocate a fixed number of cells based on the size of the type.
   - **Arrays**: Recursively allocate cells for each element in the array.
   - **Structures**: Recursively allocate cells for each field within the structure.
3. **Store Variable in Stack Frame**: Place the allocated cells within the current stack frame.

### Implementation in Code

Below is the implementation of the `declare_variable` function, which handles the allocation of variables in the stack frame:

```c
void declare_variable(stack_frame *frame, int index_type_declaration) {
    if (is_declaration_base_type(index_type_declaration)) {
        allocate_cells_to_stack_frame(
            frame,
            get_declaration_description(index_type_declaration),
            get_declaration_execution(index_type_declaration)
        );
        return;
    }

    if (get_declaration_nature(index_type_declaration) == TYPE_ARRAY) {
        int element_type = get_array_element_type(index_type_declaration);
        int size = get_array_size(index_type_declaration);

        for (int i = 0; i < size; i++) {
            declare_variable(frame, element_type);
        }
    } 
    else if (get_declaration_nature(index_type_declaration) == TYPE_STRUCT) {
        int field_count = get_struct_field_count(index_type_declaration);

        for (int i = 0; i < field_count; i++) {
            int field_type = get_struct_nth_field_declaration(index_type_declaration, i);
            declare_variable(frame, field_type);
        }
    }
}
```

- **Base Types**: Directly allocate the required number of cells.
- **Arrays**: Recursively call `declare_variable` for each element.
- **Structures**: Recursively call `declare_variable` for each field.

> [!NOTE]
> For simplicity, the base types have a size of 1 in our implementation. Thus, only one cell is allocated for each base type.

The `handle_variable_declaration` function is used to initiate the declaration process:

```c
void handle_variable_declaration(int index_declaration) {
    int index_type_declaration = get_declaration_description(index_declaration);
    declare_variable(peek_execution_stack_as_mutable(), index_type_declaration);
}
```

Thanks to the compilation process, many values are stored inside the symbole tables, enabling quick access to the stored data, for example ```get_struct_field_count()``` helps us to get the number of fields in a structure:

```c
int get_struct_field_count(int struct_declaration) {
    int struct_representation = get_declaration_description(struct_declaration);
    return get_representation_value(struct_representation);
}
```

### Handling Nested Structures and Arrays

- **Nested Arrays**: For multi-dimensional arrays, the function calls itself recursively to allocate memory for each dimension.
- **Nested Structures**: For structures containing other structures or arrays, the function recursively allocates memory for each field.

---

## Address Computation

To correctly access or modify a variable, array element, or structure field, the VM needs to compute the exact memory address where the data is stored. This address is determined based on the indices provided during access and the dimensional information from the declaration table.

### Mathematical Formulas for Address Computation

#### 1. Address Calculation for Simple Variables

To compute the address of a variable, we consider two main factors: **execution offset** and **NIS declaration** (Static Nesting Level). The address calculation will depend on the region in which the variable is declared, and if the region is the global region or local, we adjust the computation accordingly.

```c
int get_variable_address(int index_declaration) {
    int region_declaration = get_declaration_region(index_declaration);
    int execution_offset = get_declaration_execution(index_declaration);
    int nis_declaration = get_region_nis(region_declaration);

    if (region_declaration == 0) return execution_offset;
    else return execution_offset - (nis_declaration + 1);
}
```

During the compilation process, we computed the execution offsets for each declaration:

```c
void declaration_variable_start(int index_lexeme_lexicographic, int index_type_lexicographic) {
    check_type_definition(index_type_lexicographic);
    check_variable_redefinition(index_lexeme_lexicographic);
    
    int index_type_declaration = get_arr_struct_declaration_index(index_type_lexicographic);
    int execution = get_declaration_execution(index_type_declaration); 

    insert_declaration_var(index_lexeme_lexicographic, peek_region(), index_type_declaration, get_region_size(get_current_region_id()));
    update_region_size(get_current_region_id(), get_region_size(get_current_region_id()) + execution);
}
```

The execution offset is updated based on the current region size, we can write the formula for calculating the address of a variable:

\[ \text{Address} = \text{execution\_offset} - (\text{nis\_declaration} + 1) \quad \text{if region\_declaration is non-zero} \]

Otherwise, if the region declaration is 0 (i.e., it's in the global scope), the address is simply the **execution offset**:

\[ \text{Address} = \text{execution\_offset} \quad \text{if region\_declaration is 0} \]

##### Explanation of the Formula

1. **execution_offset**: This represents the execution offset of the variable's type. In the case of a base type (like an integer or character), it will typically have a size of 1, representing its base memory requirement during execution.

2. **nis_declaration**: The Static Nesting Level (NIS) value represents an offset specific to the region's declaration, indicating how the regions interact or affect the variable's placement in memory.
   - **If region_declaration is 0**: This suggests the variable is declared in a global region or has no special NIS to adjust for, so the **execution offset** is directly used for the address.
   - **If region_declaration is non-zero**: This means the variable is declared in a specific region. We subtract the **NIS declaration** plus an additional offset to properly locate the variable relative to the start of the region. The **+1** adjustment is for internal alignment.

#### 2. Address Calculation for Multi-Dimensional Arrays

To compute the address of an element in a multi-dimensional array \( A \):

\[
\text{Address}(A[I_1, I_2, \ldots, I_n]) = B + \sum_{i=1}^n \left( (I_i - L_i) \times S_i \right)
\]

With \( S_i \) is the stride of the \( i^{\text{th}} \) dimension

\[ S_i = \prod_{j=i+1}^n (U_j - L_j + 1) \times E \]

And Where:

- \( B \): Base address of the array.
- \( I_i \): Index in the \( i^{\text{th}} \) dimension.
- \( L_i \): Lower bound of the \( i^{\text{th}} \) dimension.
- \( E \): Size of each element.
- \( U_i \): Upper bound of the \( i^{\text{th}} \) dimension.

#### 3. Address Calculation for Struct Fields

For a field \( f_k \) in a structure \( S \):

\[
\text{Address}(S.f_k) = B + O_{f_k}
\]

Where:

- \( B \): Base address of the structure.
- \( O_{f_k} \): Offset of field \( f_k \), calculated as:

\[
O_{f_k} = \sum_{i=1}^{k-1} \text{Size of Field } f_i
\]

#### 4. Combined Address Calculations

For arrays of structs or structs containing arrays, the address computations are combined:

- **Arrays of Structs**:

\[
\text{Address}(A[I].f_k) = B + \left( (I - L_I) \times T_S \right) + O_{f_k}
\]

Where:

- \( T_S \): Total size of the struct.
- \( O_{f_k} \): Offset of field \( f_k \) within the struct.

- **Nested Arrays in Structs**:

\[
\text{Address}(S.A[I_1, I_2, \ldots, I_n]) = B + O_A + \sum_{i=1}^n \left( (I_i - L_i) \times S_i \right)
\]

Where:

- \( O_A \): Offset of array \( A \) within the structure \( S \).

---

### Examples

#### Example 1: Multi-Dimensional Array

Consider an array \( A[3][4] \) with:

- Base address \( B = 1000 \).
- Dimensions:
  - \( L_1 = 0 \), \( U_1 = 2 \) (dimension size \( 3 \)).
  - \( L_2 = 0 \), \( U_2 = 3 \) (dimension size \( 4 \)).
- Element size \( E = 4 \) bytes (e.g., `int`).

Compute the address of element \( A[1][2] \):

1. Calculate strides:

   - \( S_1 = (U_2 - L_2 + 1) \times E = (3 - 0 + 1) \times 4 = 16 \).
   - \( S_2 = E = 4 \).

2. Compute address:

   \[
   \begin{align*}
   \text{Address} &= B + (I_1 - L_1) \times S_1 + (I_2 - L_2) \times S_2 \\
   &= 1000 + (1 - 0) \times 16 + (2 - 0) \times 4 \\
   &= 1000 + 16 + 8 = 1024
   \end{align*}
   \]

#### Example 2: Structs in Arrays

Consider an array of structs \( S[5] \):

- Base address \( B = 2000 \).
- Struct size \( T_S = 12 \) bytes.
- Field \( f \) offset \( O_f = 4 \).

Compute the address of \( S[3].f \):

1. Compute address:

   \[
   \begin{align*}
   \text{Address} &= B + (I - L_I) \times T_S + O_f \\
   &= 2000 + (3 - 0) \times 12 + 4 \\
   &= 2000 + 36 + 4 = 2040
   \end{align*}
   \]
