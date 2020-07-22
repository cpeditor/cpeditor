/* REXX */                                                         
ASCB_PTR = C2X(STORAGE(D2X(X2D("224")),4))                         
SAY X2B(ASCB_PTR)                                                  
/* COMMENTS */                                                     
/******************************************************************
   END OF COMMENTS */                                              
SAY 'ENTER YOUR NAME'                                              
PULL NAME                                                          
DO WHILE NAME \= 'QUIT'                                            
   SAY 'HELLO THERE' NAME                                          
   PULL NAME                                                       
END                                                                
X = OUTTRAP('LU.')                                                 
"LU"                                                               
X = OUTTRAP('OFF')                                                 
SAY 'THE NUMBER OF LINES:' LU.0                                    
DO I = 1 TO LU.0                                                   
  SAY LU.I /* SAYS THE OUTPUT FROM LU */                           
END                                                                
SAY 'GOOD-BYE.'                                                    
IF DATE('D') \= "13" THEN DO                                       
  Y = TESTSELECT(15,'RAD')                                         
  SAY Y                                                            
  SAY Y || " <-- TESTSELECT"               
END                                        
EXIT 1                                     
/* FUNCTIONS */                            
TESTSELECT: PROCEDURE /* TESTING SELECT */ 
 PARSE ARG NUM CAR .                       
 SELECT                                    
   WHEN NUM < 25 THEN                      
     STATUS = "25%"                        
   WHEN NUM < 50 THEN                      
     DO                                    
       IF CAR = 'RAD' THEN                 
          STATUS = 'DOPE'                  
       ELSE                                
          STATUS = 'SICK'                  
     END                                   
   WHEN NUM < 200 THEN DO                  
     NOP                                   
   END                                     
   WHEN NUM < 75 THEN                      
     STATUS = 'SEVENTY FIVE'               
   OTHERWISE                               
     IF CAR = 'RAD' THEN                   
       STATUS = 'COOL'
      STATUS = 'COOL'   
    ELSE                
      STATUS = 'RADICAL'
END                     
RETURN STATUS           
