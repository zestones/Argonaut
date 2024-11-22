# Next Steps

This document outlines the next steps for the project, we will track the progress of the project by updating the tasks as they are completed.

## **AST Nodes To-Do List**

This list tracks the development of the AST nodes for the project. Nodes that are completed are marked with a check (✔️), while pending tasks are marked with a cross (❌). Each node has been categorized for better clarity.

<details>
<summary style="font-weight: bold; font-size: 1.2em;">✨ Completed Nodes (✔️)</summary>

These nodes have been successfully implemented and are ready to be used:

### **Program Structure:**
- ✔️ **A_PROGRAM**  

### **Declarations:**
- ✔️ **A_DECLARATION_LIST**  
- ✔️ **A_VARIABLE_DECLARATION**  
- ✔️ **A_PROCEDURE_DECLARATION**  
- ✔️ **A_FUNCTION_DECLARATION**  
- ✔️ **A_TYPE_DECLARATION**  
- ✔️ **A_TYPE_ARRAY_DECLARATION**  
- ✔️ **A_TYPE_STRUCT_DECLARATION**  
- ✔️ **A_STRUCT_FIELD_LIST**  
- ✔️ **A_STRUCT_FIELD**  

### **Parameters:**
- ✔️ **A_PARAMETER_LIST**  
- ✔️ **A_PARAMETER**  

### **Statements:**
- ✔️ **A_STATEMENT_LIST**  
- ✔️ **A_RETURN_STATEMENT**
- ✔️ **A_ASSIGNMENT_STATEMENT**  
- ❌ **A_FUNCTION_CALL_STATEMENT**  

##### **Expression and Condition Nodes:**
- ✔️ **A_CONDITION**  
- ✔️ **A_AND_CONDITION**  
- ✔️ **A_OR_CONDITION**  
- ✔️ **A_NOT_CONDITION**  

##### **Operator Nodes:**
- ✔️ **A_EQUAL_OP**  
- ✔️ **A_LESS_THAN_OP**  

##### **Control Flow Nodes:**
- ✔️ **A_IF**
- ✔️ **A_IF_ELSE**  
- ✔️ **A_WHILE**

##### **Assignment Nodes:**
- ✔️ **A_VARIABLE_ASSIGNMENT**  
- ✔️ **A_ARRAY_ASSIGNMENT**  
- ✔️ **A_STRUCT_ASSIGNMENT**  

##### **Access Nodes:**
- ✔️ **A_STRUCT_FIELD_ACCESS**
- ✔️ **A_ARRAY_ACCESS**
- ✔️ **A_ARRAY_INDEX_LIST**  
- ✔️ **A_ARRAY_INDEX**  


</details>

<details>
<summary style="font-weight: bold; font-size: 1.2em;">🚧 Pending Nodes (❌)</summary>

##### **Expression and Condition Nodes:**
- ❌ **A_ADDITION**  
- ❌ **A_SUBTRACTION**  
- ❌ **A_MULTIPLICATION**  
- ❌ **A_DIVISION**  

##### **Operator Nodes:**
- ❌ **A_NOT_EQUAL_OP**  
- ❌ **A_GREATER_THAN_OP**  
- ❌ **A_LESS_EQUAL_OP**  
- ❌ **A_GREATER_EQUAL_OP**  
- ❌ **A_ADD_OP**  
- ❌ **A_SUB_OP**  
- ❌ **A_MUL_OP**  
- ❌ **A_DIV_OP**  

##### **Literal Nodes:**
- ❌ **A_INTEGER_LITERAL**  
- ❌ **A_FLOAT_LITERAL**  
- ❌ **A_BOOLEAN_LITERAL**  
- ❌ **A_CHARACTER_LITERAL**  
- ❌ **A_STRING_LITERAL**  
- ❌ **A_IDENTIFIER**  

##### **Access Nodes:**
<!-- NOT NEEDED ? -->
- ❌ **A_ARRAY_FIELD_ACCESS**  
- ❌ **A_STRUCT_ARRAY_ACCESS**  

##### **I/O Nodes:**
- ❌ **A_PRINT_STATEMENT**  
- ❌ **A_INPUT_STATEMENT**  

##### **Assignable Entity Nodes:**
- ❌ **A_ASSIGNABLE_ENTITY**  
- ❌ **A_ARRAY_ASSIGNABLE_ENTITY**  
- ❌ **A_STRUCT_ASSIGNABLE_ENTITY**  
- ❌ **A_ASSIGNABLE_ENTITY_LIST**  

##### **Function Argument Nodes:**
- ❌ **A_ARGUMENT_LIST**

</details>

## **Error Handling To-Do List**

This list tracks the development of error handling mechanisms for the project. Errors that are handled are marked with a check (✔️), while pending tasks are marked with a cross (❌).

<details>
<summary style="font-weight: bold; font-size: 1.2em;">✨ Completed Error Handlings (✔️)</summary>

These error handling mechanisms have been successfully implemented:

### **Variable Declarations:**
- ✔️ **Type Undefined**
- ✔️ **Variable Not Declared**
- ✔️ **Variable Redefinition:** Declaring a variable in the same scope with a duplicate name.

### **Parameters:**
- ✔️ **Base Type**
- ✔️ **Type Undefined**

</details>

<details>
<summary style="font-weight: bold; font-size: 1.2em;">🚧 Pending Error Handlings (❌)</summary>

These error handling mechanisms are yet to be implemented:

##### **General Errors:**
- ❌ **Function Argument Mismatch:** Calling a function with the wrong number of arguments or argument types.
- ❌ **Invalid Operation on Types:** Trying to multiply two pointers, access a property of a non-struct type, etc.


##### **Assignments:**
- ❌ **Type Mismatch:** Assigning a string to an integer variable (e.g., `int x = "hello";`).

##### **Expressions:**
- ❌ **Type Mismatch (Arithmetic):** (e.g., `int + string`)
- ❌ **Type Mismatch (Logical):** (e.g., `int && string`)

</details>