### Return Value Cell: The Function's Final Answer

The return value cell serves as a temporary holding area for function results:

1. **For Functions**:  
   - Mandatory cell allocated at frame creation  
   - Type-checked against function signature  
   - Value must be set before function exit  

2. **For Procedures**:  
   - No return cell allocated  
   - Attempting to return a value triggers runtime error  

The caller retrieves this value immediately after callee frame destruction, ensuring no dangling references.