## Memory Allocation in Stack Frames

When a variable is declared within a function, the system retrieves its size from the declaration table and allocates memory within the current stack frame.

### Variable Declaration and Allocation

When a variable is declared within a function:

1. **Identify Type**: Determine if the variable is a base type, array, or structure.
2. **Allocate Memory**:
   - **Base Types**: Allocate a fixed number of cells based on the type.
   - **Arrays**: Allocate cells for each array element based on the size of each element.
   - **Structures**: Allocate cells for each field within the structure.
3. **Store in Stack Frame**: Place the allocated cells within the current stack frame, initializing them as needed.

**Function**: `declare_variable(stack_frame *frame, int index_type_declaration)`

### Handling Nested Structures and Arrays

- **Nested Arrays**: Allocate memory for multi-dimensional arrays by recursively allocating cells for each dimension.
- **Nested Structures**: Allocate memory for structures containing other structures or arrays by traversing the nested declarations and allocating appropriate cells.
- **Function**: `handle_variable_declaration(int index_declaration)`
