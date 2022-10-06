# include <iostream>
# include <stdio.h>
# include <vector>
# include <string>
# include <exception>
# include <sstream>
# include <iomanip>

using namespace std ;
  // �Ĥ@���u Unrecognized token with first char ���L�k�ѧO��Token eg $ 

  // �ĤG���u(token recognized, parse grammar) Unexpected token 

  // �ĤT���u(grammar OK, evaluate it) Undefined identifier

class Form // �C��Token���ҰO�����F�� 
{
  public:
  string mtoken ; // �O��token  
  string mtype ;  // �O������
  string mvalue ; // �O����
  int minstruction_line_num ; // �O����Token��line�� 
  int minstruction_column_num ; // �O����Token��column�� 
}; // Form


class Scanner // �D�n�@�άO��InputŪ�J,�ä���Token,�x�s�bInput_instruction
{
  int mcurrent_line_num, mcurrent_column_num ; // �O���{�ɦ��
  public:
  // int current_instruction_line_num, current_instruction_column_num ; // �O���{�ɦ�� 
  string mtemp_input ; // �ȦsInput�� 
  char mtemp_char ; 
  vector< vector<Form> > mInput_Instruction_Set ;
  vector<Form> mInput_Instruction ; // �u��@��Token 
  // �Q�k�Oinput_instruction_Set[]��vector�x�s�@���input�����O
  // ��input_instruction[]�Oinput���O�@�ӭ�(token)�x�s
  // eg. input a+b-c;                           1   +    2    \n
  // Input_Instruction_Set[0][0]:abc              3   ;  2 + 1  \n
  //                                              1 + 5 ; 2 - a \n
  //                                              ;
  // Input_Instruction_Set[0][1]:+              Input_Instruction_Set[0][0]: 1    
  // Input_Instruction_Set[0][2]:12             Input_Instruction_Set[0][2]: +
  // Input_Instruction_Set[0][3]:-              Input_Instruction_Set[0][3]: 2
  // Input_Instruction_Set[0][4]:de             
  // Input_Instruction_Set[0][5]:;
  //                          ^
  //                          |
  //                 Input_Instruction
  
  //  Token��9��
  //    [SIGN]NUM  SIGN �i���i�L,���u��O+-!  NUM�}�Y�i�H�O.�᭱�O�Ʀr or 
  // �}�Y�i�H�O�Ʀr,�᭱�O. ��.�u��X�{�@�� or ��¬O�Ʀr 
  //    IDENT  �@�w�n�O�r���}�Y, �᭱�i�H�O�r���B�Ʀr�B_ 
  //    SYMBOL  �Ÿ� + - * / ;
  //    ASSIGN := 
  //    BBOOL_OPERATOR = <> < > >= <= 
  //    LEFT_BRACKET ( 
  //    RIGHT_BRACKET )  
  //    QUIT  ���} (�S�O�Ҥl) 
  //    Command ���� �ڨϥ� isCommand �@��return Token��
  
  char Get_next_char() // Ū�J�U��char(Ū��space) 
  {
    // �t�dkeep track of��get�쪺char��line number and column number
    char next_char ;
    while ( cin.peek() == ' ' || cin.peek() == '\t' )
    {
      // cout << "It is space\n" ;
      scanf( "%c", &next_char ) ;
    } // while


    scanf( "%c", &next_char ) ;
    return next_char; 
 
      
  } // Get_next_char()

  char Get_next_non_white_char() // Ū�J�U��first char of Token 
  {

    mtemp_char = Get_next_char() ;
    return mtemp_char ;
  } // Get_next_non_white_char()
  
  void Def_Token( char input_char ) // �t�d�T�w�O����Token
  {
    string token ;
    string command ;
    string error ;
    Form token_form ;
    // cout << "Start Def_Token\n" ;
    char temp_input_char ; // �Χ@����Ū�� 
    if ( ( input_char >= '0' && input_char <= '9' ) || input_char == '.' ) // �P�_�O�_NUM 
    {
      token = Def_Token_NUM( input_char ) ;
      token_form = Set_Token_Information( token ) ;
      mcurrent_column_num++ ;
      mInput_Instruction.push_back( token_form ) ;
    } // if
    else if ( ( input_char >= 'a' && input_char <= 'z' ) || ( input_char >= 'A' && input_char <= 'Z' ) )
    {
      token = Def_Token_IDENT( input_char ) ;
      token_form = Set_Token_Information( token ) ;
      mcurrent_column_num++ ;
      mInput_Instruction.push_back( token_form ) ;
    } // else if 
    else if ( input_char == '+' || input_char == '-' || input_char == '*' 
              || input_char == '/' || input_char == ';'  ) 
    // �P�_�O�_SYMBOL 
    {
      if ( input_char == '/' )
      {
        if ( cin.peek() == '/' ) // ��X���� 
        {
          command = input_char ;
          while ( cin.peek() != '\n' )
          {
            scanf( "%c", &temp_input_char ) ;
            command = command + temp_input_char ;
          } // while      
            
          scanf( "%c", &temp_input_char ) ; // Ū������ 
          command = command + temp_input_char ;
          mcurrent_line_num++ ;
          mcurrent_column_num = 0 ;
        } // if
        else // �O���k 
        {
          token = Def_Token_SYMBOL( input_char );
          token_form = Set_Token_Information( token ) ;
          mcurrent_column_num++ ;
          mInput_Instruction.push_back( token_form ) ;
        } // else 
      } // if
      else 
      {
        token = Def_Token_SYMBOL( input_char );
        token_form = Set_Token_Information( token ) ;
        mcurrent_column_num++ ;
        mInput_Instruction.push_back( token_form ) ;  
      } // else 
    } // else if
    else if ( input_char == ':' ) // �P�_�O�_ASSIGN
    {
      token = Def_Token_ASSIGN( input_char );
      token_form = Set_Token_Information( token ) ;
      mcurrent_column_num++ ;
      mInput_Instruction.push_back( token_form ) ;
    } // else if
    else if ( input_char == '=' ) // �P�_�O�_= BOOL_OPRATOR���@��  
    {
      token = Def_Token_BOOL_OPERATOR( input_char );
      token_form = Set_Token_Information( token ) ;
      mcurrent_column_num++ ;
      mInput_Instruction.push_back( token_form ) ;
    } // else if 
    else if ( input_char == '<' || input_char == '>' ) // �P�_�O�_< > �}�Y��BOOL_OPRATOR
    {
      token = Def_Token_BOOL_OPERATOR( input_char );
      token_form = Set_Token_Information( token ) ;
      mcurrent_column_num++ ;
      mInput_Instruction.push_back( token_form ) ;
    } // else if
    else if ( input_char == '(' || input_char == ')' ) 
    {
      token = Def_Token_BRACKET( input_char );
      token_form = Set_Token_Information( token ) ;
      mcurrent_column_num++ ;
      mInput_Instruction.push_back( token_form ) ;
    } // else if  
    else if ( input_char == '\n' )  // �P�_�O�_\n
    {
      mcurrent_line_num++ ;
      mcurrent_column_num = 0 ;
      mInput_Instruction.clear() ;
    } // else if
    else 
    {
      error = input_char ;  
      throw  "Unrecognized token with first char : '" + error + "'" ;
    } // else
    // cout << "End Def_Token\n" ;  
  } // Def_Token()
  
  string Def_Token_NUM( char input_char ) // �T�O�ONUM��Token �O�afirst_char�h�P�_, �ñqinput��Ū�� 
  {
    string token_num ; // Return ���� 
    string error_message ;
    char temp_input ;
    bool is_decimal = false ; // �O�_�p�� 
    bool non_num = false ; // �O�_�Ʀr 
    int point_counter = 0; // Keep track ���h��. 

    token_num = input_char ;
    if ( token_num == "." ) point_counter++ ;
    if ( token_num == "." ) // �p�GFirst_char�O . 
    {
      while ( non_num != true )
      {
        if ( cin.peek() >= '0' && cin.peek() <= '9' )
        {
          scanf( "%c", &temp_input ) ;
          token_num = token_num + temp_input ;  
        } // if
        else 
        {
          if ( token_num.size() == 1 && token_num == "." ) // �u�O1��.,�e�᳣�S���Ʀr 
          {
            error_message = "Unrecognized token with first char : '" + token_num + "'" ;
            throw error_message ;
          } // if
          else // �J��D�Ʀr���r�� 
          {
            non_num = true ;
            return token_num ;  
          } // else   
        } // else 
      
      } // while     
    } // if
    else if ( ( token_num >= "0" && token_num <= "9" ) || token_num == "." ) // �p�GFirst_char�O�Ʀr 
    {
      while ( non_num != true )
      {
        if ( cin.peek() == '.' ) // �U�@�ӬO�p��. 
        {
          if ( point_counter < 1 ) // �٥��X�{�p��. 
          {
            scanf( "%c", &temp_input ) ;
            token_num = token_num + temp_input ;
            point_counter++ ;
          } // if
          else // �w�g�O�Ӥp�� 
          {
            return token_num ;   
          } // else    
        } // if
        else if ( cin.peek() >= '0' && cin.peek() <= '9' )
        {
          scanf( "%c", &temp_input ) ;
          token_num = token_num + temp_input ;  
        } // else if
        else // ���O�Ʀr�M�p���I. 
        {
          return token_num ;  
        } // else    
      } // while     
    } // else if

    return token_num;
  } // Def_Token_NUM()

  string Def_Token_IDENT( char input_char ) // �T�O�OIDENT��Token �O�afirst_char�h�P�_, �ñqinput��Ū�� 
  {
    string token_id ;
    char temp_char ;
    token_id = input_char ; // Return���� 
    while ( ( cin.peek() >= 'a' && cin.peek() <= 'z' ) || ( cin.peek() >= 'A' && cin.peek() <= 'Z' ) 
            || ( cin.peek() >= '0' && cin.peek() <= '9' ) || cin.peek() == '_' )
    { 

      if ( ( cin.peek() >= 'a' && cin.peek() <= 'z' ) || ( cin.peek() >= 'A' && cin.peek() <= 'Z' )
           || ( cin.peek() >= '0' && cin.peek() <= '9' ) || cin.peek() == '_' ) 
      {
        scanf( "%c", &temp_char ) ;
        token_id = token_id + temp_char; 
      } // if
      else
      {
        return token_id ;  
      } // else
       
    } // while

    return token_id ;
  } // Def_Token_IDENT()
  
  string Def_Token_SYMBOL( char input_char ) // �T�O�OSYMBOL��Token �O�afirst_char�h�P�_, �ñqinput��Ū�� 
  {
    string token_symbol ;
    token_symbol = input_char;
    return token_symbol;
  } // Def_Token_SYMBOL()
  
  string Def_Token_ASSIGN( char input_char ) // �T�O�OASSIGN��Token �O�afirst_char�h�P�_, �ñqinput��Ū�� 
  {
    string token_assign ; // �i�Ӫ�first_char�@�w�O:
    char temp_input ; 
    string error ;
    token_assign = input_char ;
    if ( cin.peek() == '=' ) // �U�@�ӬO=, �N�O:=���� 
    {
      scanf( "%c", &temp_input ) ;
      // cout << "here\n" ;
      return token_assign = token_assign + temp_input ; 
    } // if
    else // �U�@�Ӥ��O=,�N�O�Ĥ@���u Unrecognized token with first char 
    {
      error = input_char ;
      throw  "Unrecognized token with first char : '" + error + "'" ; 
    } // else
  } // Def_Token_ASSIGN()
  
  string Def_Token_BOOL_OPERATOR( char input_char ) // �T�O�OBOOL_OPERATOR��Token
  // �O�afirst_char�h�P�_, �ñqinput��Ū��
  {
    string token_bool_operator ; // �i�Ӫ�first_char�O = < >
    char temp_input ; 
    token_bool_operator = input_char ; 
    if ( token_bool_operator == ">"  ) // �i�Ӫ�first_char�O >
    {
      if ( cin.peek() == '=' ) // �U�@�ӬO=, �N�O >=  
      {
        scanf( "%c", &temp_input ) ;
        return token_bool_operator = token_bool_operator + temp_input; 
      } // if
      else return token_bool_operator ; // �N�O >
    } // if
    else if ( token_bool_operator == "<" )  
    {
      if ( cin.peek() == '=' ) // �U�@�ӬO=, �N�O <=  
      {
        scanf( "%c", &temp_input ) ;
        return token_bool_operator = token_bool_operator + temp_input; 
      } // if
      else if ( cin.peek() == '>' ) // �U�@�ӬO>, �N�O <>
      {
        scanf( "%c", &temp_input ) ;
        return token_bool_operator = token_bool_operator + temp_input;
      } // else if
      else return token_bool_operator ; // �N�O <  
    } // else if
    else if ( token_bool_operator == "=" )
    {
      return token_bool_operator; 
    } // else if   

    return token_bool_operator ; // PL�n�[�� 
  } // Def_Token_BOOL_OPERATOR()
  
  string Def_Token_BRACKET( char input_char ) // �T�O�OBRACKET��Token �O�afirst_char�h�P�_, �ñqinput��Ū��
  {
    string token_bracket ; // �i�Ӫ�first_char�O ( ) 
    token_bracket = input_char; 
    return token_bracket ;
  } // Def_Token_BRACKET()
  
  Form Set_Token_Information( string temp_input )
  // ����n��Token��iInput_Instruction ,******���٥���iInput_Instruction_Set********
  {
    Form basic_form ;
    bool point_check = false ;
    // cout << "start Set_Token_To_Input_Instruction \n" ;
    if ( ( temp_input[0] >= '0' && temp_input[0] <= '9' ) || temp_input[0] == '.' ) // �T�w�O�_INT or FLOAT 
    {
      for ( int i = 0; i < temp_input.size() ; i++ )
      {
        if ( temp_input[i] == '.' )
        {
          point_check = true; 
        } // if    
      } // for
      
      if ( point_check == false ) // �Oint 
      {
        basic_form.mtoken = temp_input ;
        basic_form.mtype = "INT" ;
        basic_form.mvalue = temp_input;
        basic_form.minstruction_line_num = mcurrent_line_num ;
        basic_form.minstruction_column_num = mcurrent_column_num ;
      } // if
      else  // �Ofloat 
      {
        basic_form.mtoken = temp_input ;
        basic_form.mtype = "FLOAT" ;
        basic_form.mvalue = temp_input;
        basic_form.minstruction_line_num = mcurrent_line_num ;
        basic_form.minstruction_column_num = mcurrent_column_num ; 
      } // else   

      // mInput_Instruction.push_back( basic_form ) ;
    
      // cout << "end Set_Token_To_Input_Instruction num \n" ;
      return basic_form ;
    } // if
    else if ( ( temp_input[0] >= 'a' && temp_input[0] <= 'z' ) || 
              ( temp_input[0] >= 'A' && temp_input[0] <= 'Z' ) ) // �T�w�OIDENT
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "IDENT" ;
      basic_form.mvalue = "" ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      // mInput_Instruction.push_back( basic_form );
      // cout << "end Set_Token_To_Input_Instruction id\n" ;
      return basic_form ;
    } // else if
    else if ( temp_input == "+" || temp_input == "-" || 
              temp_input == "*" || temp_input == "/" ) // �T�w�O SYMBOL
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "SYMBOL" ;
      basic_form.mvalue = temp_input ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      // mInput_Instruction.push_back( basic_form );
      // cout << "end Set_Token_To_Input_Instruction SYMBOL\n" ; 
      return basic_form ;
    } // else if
    else if ( temp_input == ":=" ) // �T�w�OASSIGN
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "ASSIGN" ;
      basic_form.mvalue = temp_input ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      // cout << "end Set_Token_To_Input_Instruction ASSIGN\n" ; 
      return basic_form ;
      // mInput_Instruction.push_back( basic_form ); 
    } // else if
    else if ( temp_input == ">" || temp_input == ">=" || temp_input == "<" 
              || temp_input == "<=" || temp_input == "<>" || temp_input == "=" ) // �T�w�OBOOL_OPERATOR 
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "BOOL_OPERATOR" ;
      basic_form.mvalue = temp_input ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      // cout << "end Set_Token_To_Input_Instruction BOOL_OPERATOR\n" ; 
      return basic_form ;
      // mInput_Instruction.push_back( basic_form ); 
    } // else if
    else if ( temp_input == "(" ) // �T�w�OLEFT_BRACKET
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "LEFT_BRACKET" ;
      basic_form.mvalue = temp_input ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      // cout << "end Set_Token_To_Input_Instruction LEFT_BRACKET\n" ; 
      return basic_form ;
      // mInput_Instruction.push_back( basic_form );
    } // else if
    else if ( temp_input == ")" ) // �T�w�ORIGHT_BRACKET
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "RIGHT_BRACKET" ;
      basic_form.mvalue = temp_input ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      // cout << "end Set_Token_To_Input_Instruction RIGHT_BRACKET\n" ; 
      return basic_form ;
      // mInput_Instruction.push_back( basic_form );
    } // else if
    else if ( temp_input == ";" ) // �T�w�O; 
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "SYMBOL" ;
      basic_form.mvalue = temp_input ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      // mInput_Instruction.push_back( basic_form );
      // cout << "end Set_Token_To_Input_Instruction ;\n" ; 
      return basic_form ;
    } // else if  
    else  // ���OUnrecognized 
    {
      basic_form.mtoken = temp_input;
      basic_form.mtype = "Unrecognized" ;
      basic_form.mvalue = temp_input ;
      basic_form.minstruction_line_num = mcurrent_line_num ;
      basic_form.minstruction_column_num = mcurrent_column_num ;
      return basic_form ;
      // mInput_Instruction.push_back( basic_form ) ;
    } // else 
  } // Set_Token_Information()
    
  Form Get_INPUT()
  {
    char input_key ; // �x�sŪ�J���r��
    string token ;
    Form temp_input_token ; 
    bool exist_from_input = false ;
    bool error = false ; 
    mcurrent_line_num = 0;
    mcurrent_column_num = 0 ;
    while ( exist_from_input != true )
    {
      input_key = Get_next_non_white_char() ; // Ū�J�@�ӫD�ťզr��, �������� 
      Def_Token( input_key ) ; // ��Ū�J���e��Def_Token�@�P�_  
      if ( mInput_Instruction.size() == 1 )
      {
        exist_from_input = true ;
        temp_input_token = mInput_Instruction[0] ;
        mInput_Instruction.clear() ;
        return temp_input_token ;
      } // if
    } // while

    return temp_input_token ;
    // cout << "Out\n" ;
  } // Get_INPUT()
  
  void Clear_line()
  {
    char waste ;
    // cout << "Clear start\n" ;
    // waste = cin.peek() ;
    while ( cin.peek() != '\n' )
    {
      scanf( "%c", &waste ) ;
      // cout << "Clear " << waste << endl ;  
    } // while  
    // cout << "Clear end2\n" ;
  } // Clear_line()
  
  
  Form Peek_next_token()
  {
    char temp_input ;
    char putback_char ;
    string empty ;

    temp_input = cin.peek() ;
    while ( temp_input == ' ' || temp_input == '\t' || temp_input == '\n' )
    {
      scanf( "%c", &temp_input ) ;
      temp_input = cin.peek() ;    
    } // while    
  
    // cout << "Peek is" << temp_input << "\n" ;
    // �Ofirst_char 
    Form basic_form ;
    bool point_check = false ;
    // cout << "start Peek_next_token \n" ;
    if ( ( temp_input >= '0' && temp_input <= '9' ) || temp_input == '.' ) // �T�w�O�_INT or FLOAT 
    {

      if ( temp_input == '.' )
      {
        point_check = true; 
      } // if    

      if ( point_check == false ) // �Oint 
      {
        basic_form.mtoken = temp_input ;
        basic_form.mtype = "INT" ;
      } // if
      else  // �Ofloat 
      {
        basic_form.mtoken = temp_input ;
        basic_form.mtype = "FLOAT" ;
      } // else   

      // cout << "End Peek_next_token num \n" ;
      return basic_form ;
    } // if
    else if ( ( temp_input >= 'a' && temp_input <= 'z' ) || 
              ( temp_input >= 'A' && temp_input <= 'Z' ) ) // �T�w�OIDENT
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "IDENT" ;
      // cout << "End Peek_next_token id \n" ;
      return basic_form ;
    } // else if
    else if ( temp_input == '+' || temp_input == '-' || 
              temp_input == '*' || temp_input == '/' ) // �T�w�O SYMBOL
    {
      scanf( "%c", &putback_char ) ; 
      if ( putback_char == '/' && cin.peek() == '/' )
      {
        basic_form.mtoken = temp_input + temp_input ;
        basic_form.mtype = "COMMAN" ;
        // cout << "End Peek_next_token comman \n" ;  
      } // if
      else 
      {
        basic_form.mtoken = temp_input ;
        basic_form.mtype = "SYMBOL" ;
        // cout << "End Peek_next_token symbol \n" ;
      } // else 
    
      cin.putback( putback_char ) ;
      return basic_form ;
    } // else if
    else if ( temp_input == ':' ) // �T�w�OASSIGN
    {
      scanf( "%c", &putback_char ) ; 
      if ( cin.peek() == '=' )
      {
        temp_input = cin.peek() ;
        basic_form.mtoken = ":=" ;
        basic_form.mtype = "ASSIGN" ;
        // cout << "End Peek_next_token assign \n" ;
        cin.putback( putback_char ) ;  
      } // if
      else 
      {
        cin.putback( putback_char ) ;
        basic_form.mtoken = ':' ;
        basic_form.mtype = "Unrecognized" ;
        // cout << "End Peek_next_token unrecognized \n" ;
      } // else

      return basic_form ;
      // mInput_Instruction.push_back( basic_form ); 
    } // else if
    else if ( temp_input == '>' || temp_input == '<' || temp_input == '=' ) // �T�w�OBOOL_OPERATOR 
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "BOOL_OPERATOR" ;
      // cout << "End Peek_next_token bool \n" ;
      return basic_form ;
      // mInput_Instruction.push_back( basic_form ); 
    } // else if
    else if ( temp_input == '(' ) // �T�w�OLEFT_BRACKET
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "LEFT_BRACKET" ;
      // cout << "End Peek_next_token Left Bracket \n" ;
      return basic_form ;
      // mInput_Instruction.push_back( basic_form );
    } // else if
    else if ( temp_input == ')' ) // �T�w�ORIGHT_BRACKET
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "RIGHT_BRACKET" ;
      // cout << "End Peek_next_token Right Bracket \n" ;
      return basic_form ;
      // mInput_Instruction.push_back( basic_form );
    } // else if
    else if ( temp_input == ';' ) // �T�w�O; 
    {
      basic_form.mtoken = temp_input ;
      basic_form.mtype = "SYMBOL" ;
      // cout << "End Peek_next_token ';' \n" ;
      return basic_form ;
    } // else if  
    else  // ���OUnrecognized 
    {
      basic_form.mtoken = temp_input;
      basic_form.mtype = "Unrecognized" ;
      // cout << "End Peek_next_token unrecognized \n" ;
      return basic_form ;
    } // else 
  } // Peek_next_token()
  
  void CommanReader()
  {
    char temp_input_char ;
    while ( cin.peek() != '\n' )
    {
      scanf( "%c", &temp_input_char ) ;
    } // while      
            
    scanf( "%c", &temp_input_char ) ; // Ū������
  } // CommanReader()
} ; // Scanner 

class Parser // �D�n�@�άO��k�ˬd�M�B��
{
  // Parser�D�n����Part, 1st Part ��k�ˬd, 2nd Part �B�� 
  // 1st Part ���\�~��]Part2
  /*
  <Command> ::= IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';' 
                | <NOT_IDStartArithExpOrBexp> ';' 
                | QUIT

  <IDlessArithExpOrBexp>  ::= {   '+' <Term>   | '-' <Term> 
                                | '*' <Factor> | '/' <Factor> 
                              }
                              [ <BooleanOperator> <ArithExp> ]

  <BooleanOprator>        ::= '=' | '<>' | '>' | '<' | '>=' | '<='

  <NOT_ID_StartArithExpOrBexp> 
                          ::= <NOT_ID_StartArithExp> 
                              [ <BooleanOperator> <ArithExp> ]

  <NOT_ID_StartArithExp>  ::= <NOT_ID_StartTerm> { '+' <Term> | '-' <Term> }
  <NOT_ID_StartTerm>      ::= <NOT_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
  <NOT_ID_StartFactor>    ::= [ SIGN ] NUM | '(' <ArithExp> ')'
  <ArithExp>              ::= <Term> { '+' <Term> | '-' <Term> }
  <Term>                  ::= <Factor> { '*' <Factor> | '/' <Factor> }
  <Factor>                ::= IDENT | [ SIGN ] NUM | '(' <ArithExp> ')'
 
  */
  public :
  // int current_instruction_line_num, current_instruction_column_num; // �O���{�ɦ��
  // int line_num, column_num = 0; // �O�Ω�VScanner��Input_Instruction_Set��Keep track
  vector<Form> mident_def_set ; // �O��ident 10000000*** 
  Form mtemp_calculate ;
  // bool error ; 
  vector<Form> minstruction_buffer ; // ��Get_INPUT()�^�Ǫ�token�s�_ 
  string merror_type ; // ��error�Otrue, ��error_type�O���O�ƻ�error 
  Scanner mfirst ;
  int mcLine, mcColumn ;
  int merror_line_num ;
  
  void Start_Scanner()
  {
    string error ;
    Form input_token ;
    bool is_quit = false ;
    bool is_calculable = false ;
    bool is_scanner_ok = true ;
    mcLine = 0 ;
    mcColumn = 0 ;
    cout << "Program starts...\n" ;
    while ( is_quit != true )
    {
      try
      {
        cout << "> " ;
        input_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
        // cout << endl ;
        minstruction_buffer.push_back( input_token ) ; 
      } // try 
      catch( string error_type_1 ) 
      {
        // �qScanner�Ǩ�error �OUnrecognized token with first char 
        cout << error_type_1 << endl ;
        Clear_buffer_and_line() ;  
        is_scanner_ok = false ;
      } // catch
      try
      {
        if ( is_scanner_ok == true )
        {
          if ( Check_Grammer() == false ) 
          {
            is_calculable = false ;
            mcColumn = 0 ;  
          } // if
          else 
          {
            is_calculable = true ;
            mcColumn = 0 ;  
          } // else           
        } // if
        else 
        {
          is_calculable = false ;
          is_scanner_ok = true ;
          mcColumn = 0 ;  
        } // else 
  
      } // try
      catch( string error_type_2 )
      {
        // �qScanner�Ǩ�error �OUnexpected token :
        error = error_type_2 ;
        if ( error == "quit" )
        {
          is_quit = true ;  
        } // if
        else 
        {
          cout << error_type_2 << endl ;
          Clear_buffer_and_line() ;  
        } // else   

        is_calculable = false ;
        mcColumn = 0 ;    
      } // catch 
    
      if ( is_calculable == true )
      {
        try
        {
          // cout << "calculable\n" ;
          Calculation() ;
          mcColumn = 0 ;
          minstruction_buffer.clear() ;  
        } // try
        catch( string error_type_3 )
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          cout << error_type_3 << endl ;
          Clear_buffer_and_line() ;
          is_calculable = false ;
          mcColumn = 0 ;    
        } // catch  
      } // if
    
    } // while
    
    cout << "Program exits..." << endl ;
  } // Start_Scanner()
  
  bool Check_Grammer() // �qminstruction_buffer�����ˬd��k (�֤�Function) Parser1
  {
    if ( Check_Command_Grammer() == false ) 
    {
      return false ;  
    } // if
    else 
    {
      return true ;  
    } // else   
  } // Check_Grammer()
 
  bool Check_Command_Grammer() // �ˬdCommand��Grammer
  {
    Form temp_Instruction_token ;
    string id_token ;
    bool is_ident_def = false ;
    bool id_arith = false ;
    bool not_id = false ;
    string error ;
    string quit = "quit" ;
    // cout << "Start Check_Command_Grammer\n" ;
    // �ˬd IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'
    
    // ��mcColumn�Mminstruction_buffer.size()�۵�,�N��ǳ�Ū�J�sToken
    // mcColumn��minstruction_buffer.size()�p,�N��token���Q�P�w,�̵M�O�W��Ū�J��
   
    if ( minstruction_buffer[mcColumn].mtype == "IDENT" )
    {
      // cout << "It is IDENT " << minstruction_buffer[mcColumn].mtoken << endl ;
      if ( minstruction_buffer.size() == 1 && minstruction_buffer[mcColumn].mtoken == "quit" )
      {
        throw quit ;  
      } // if
    
      id_token = minstruction_buffer[mcColumn].mtoken ;
      mcColumn++ ; // �O�ocurrent_instruction_column_num++ �O�ѩI�s��/�W�h�t�d
      temp_Instruction_token = mfirst.Peek_next_token() ;
      while ( temp_Instruction_token.mtype == "COMMAN" )
      {
        // cout << "CommanReader\n" ;
        mfirst.CommanReader() ;
        temp_Instruction_token = mfirst.Peek_next_token() ;
      } // while

      // cout << "Now Peek :" << temp_Instruction_token.mtoken << endl ;
      /*
      if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
      {
        try
        {
          cout << "Input 2\n" ;
          temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
          minstruction_buffer.push_back( temp_Instruction_token ) ; 
        } // try 
        catch( string error_type_1 ) 
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          // cout << error_type_1 << endl ;
          error = error_type_1 ;
          Clear_buffer_and_line() ;
          throw error ;
          return false ;   
        } // catch
      } // if 
      */
      //  ':=' <ArithExp>
      if ( temp_Instruction_token.mtype == "ASSIGN" )  
      {
        // cout << "It is ASSIGN :=\n" ;
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            // cout << "Input 2\n" ;
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
      
        mcColumn++ ;
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            // cout << "input 3\n" ;
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;  
          } // catch
        } // if
      
        if ( Check_ArithExp_Grammer() )  
        {
          // cout << "Return to Check_Command_Grammer1\n" ;
        } // if
        else // �bCheck_ArithExp()�X�{ERROR 
        {  
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'";
          throw error ;
        } // else  
        
        

        // cout << "IDENT ( ':=' <ArithExp> \n" ;
      } // if

      // <IDlessArithExpOrBexp> 
      else if ( temp_Instruction_token.mtype == "SYMBOL" || temp_Instruction_token.mtype == "BOOL_OPERATOR" )
      {
        if ( Check_ID_Def( id_token ) )
        {
          // cout << "<IDlessArithExpOrBexp>\n" ;
          if ( Check_IDlessArithExpOrBexp_Grammer() )
          {
            // cout << "Return to Check_Command_Grammer2\n" ;
          } // if
          else 
          {
            error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'";
            throw error ;  
          } // else  
        } // if 
        else 
        {
          error = "Undefined identifier : '" + id_token + "'" ;
          throw error ;  
        } // else 
      } // else if  
     
      // ';'
      // cout << mcColumn << endl ;
      // cout << minstruction_buffer.size() << endl ;
      if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
      {
        // cout << "in;\n" ;
        try
        {
          // cout << "input 4\n" ; 
          temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
          minstruction_buffer.push_back( temp_Instruction_token ) ; 
        } // try 
        catch( string error_type_1 ) 
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          // cout << error_type_1 << endl ;
          error = error_type_1 ;
          Clear_buffer_and_line() ;
          throw error ;
          return false ;  
        } // catch
      
        // cout << "out\n" ;
        // mcColumn++ ;
        if ( minstruction_buffer[mcColumn].mtoken == ";" )
        {
          // cout << "It is ;\n" ;
          return true ;
        } // if
        else // �I�줣�O;��Token ERROR Unexpected token
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;    
        } // else
      } // if  
    
    } // if 
               
    // �ˬd <NOT_IDStartArithExpOrBexp> ';'
    else
    {
      if ( mcColumn < minstruction_buffer.size() )
      {
        
        if ( Check_NOT_IDStartArithExpOrBexp_Grammer() )  // �ˬdNOT_IDStartArithExpOrBexp��Grammer
        {  
          // cout << "Return to Check_Command_Grammer\n" ;
          // mcColumn++ ;
        } // if
        else 
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;  
        } // else
        
        // ';'
        if ( mcColumn == minstruction_buffer.size() )
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if  
  
        if ( minstruction_buffer[mcColumn].mtoken == ";" )
        {
          // cout << "It is ;\n" ;
          return true ;  
        } // if
        else 
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;   
        } // else    
      } // if  
      
    } // else 
  
    if ( mcLine > mfirst.mInput_Instruction_Set.size() ) 
    {
      return true ;  
    } // if
    
    return false ; // PL�n�[��
  } // Check_Command_Grammer()
  
  bool Check_IDlessArithExpOrBexp_Grammer() // �ˬdIDlessArithExpOrBexp
  {
    // <IDlessArithExpOrBexp>  ::= { '+' <Term>  | '-' <Term>  | '*' <Factor> | '/' <Factor>  } 
    // [ <BooleanOperator> <ArithExp> ]
    Form temp_Instruction_token ;
    string error ;
    // cout << "Start Check_IDlessArithExpOrBexp_Grammer \n" ;
    // cout << mcColumn << endl ;
    temp_Instruction_token = mfirst.Peek_next_token() ;
    while ( temp_Instruction_token.mtype == "COMMAN" )
    {
      // cout << "CommanReader\n" ;
      mfirst.CommanReader() ;
      temp_Instruction_token = mfirst.Peek_next_token() ;
    } // while

    while ( ( ( temp_Instruction_token.mtoken == "+" ) 
              || ( temp_Instruction_token.mtoken == "-" )  
              || ( temp_Instruction_token.mtoken == "*" ) 
              || ( temp_Instruction_token.mtoken == "/" ) ) )
    {
    
      // cout << "in +|-|*|/ \n" ;
      if ( ( temp_Instruction_token.mtoken == "+" ) 
           || ( temp_Instruction_token.mtoken == "-" ) )
      {
  
        // cout << "in +|- \n" ;
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
      
        mcColumn++ ;
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        if ( Check_Term_Grammer() )
        {
          // cout << "Return to Check_IDlessArithExpOrBexp_Grammer1\n" ;
          // cout << mcColumn << endl ;
          // cout << minstruction_buffer[mcColumn].mtoken << endl ; 
        } // if
        else 
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ; 
          throw error ;  
        } // else    
      } // if  
      else if ( ( temp_Instruction_token.mtoken == "*" ) 
                || ( temp_Instruction_token.mtoken == "/" ) ) 
      {

        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        mcColumn++ ;
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        if ( Check_Factor_Grammer() )
        {
          // cout << "Return to Check_IDlessArithExpOrBexp_Grammer2\n" ;     
        } // if
        else 
        {
          error = "Unexpected token : '" +  minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;  
        } // else   
      } // else if
    
      temp_Instruction_token = mfirst.Peek_next_token() ;
      while ( temp_Instruction_token.mtype == "COMMAN" )
      {
        // cout << "CommanReader\n" ;
        mfirst.CommanReader() ;
        temp_Instruction_token = mfirst.Peek_next_token() ;
      } // while
    } // while
    
    temp_Instruction_token = mfirst.Peek_next_token() ;
    while ( temp_Instruction_token.mtype == "COMMAN" )
    {
      // cout << "CommanReader\n" ;
      mfirst.CommanReader() ;
      temp_Instruction_token = mfirst.Peek_next_token() ;
    } // while

    if ( temp_Instruction_token.mtoken == ">" 
         || temp_Instruction_token.mtoken == "<" || temp_Instruction_token.mtoken == "=" )
    {
      if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
      {
        try
        {
          temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
          minstruction_buffer.push_back( temp_Instruction_token ) ; 
        } // try 
        catch( string error_type_1 ) 
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          // cout << error_type_1 << endl ;
          error = error_type_1 ;
          Clear_buffer_and_line() ;
          throw error ;
          return false ;   
        } // catch
      } // if 

      if ( Check_BooleanOprator_Grammer() )
      {
        mcColumn++ ;
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
        
        if ( Check_ArithExp_Grammer() )
        {
          // cout << "Return to Check_IDlessArithExpOrBexp_Grammer3\n" ;  
        } // if
        else 
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;    
        } // else   
      } // if
      else 
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else 
    } // if
    
    // cout << "Check_IDlessArithExpOrBexp_Grammer OK\n" ;
    return true ; 
  } // Check_IDlessArithExpOrBexp_Grammer()
  
  bool Check_NOT_IDStartArithExpOrBexp_Grammer() // �ˬdNOT_IDStartArithExpOrBexp��Grammer
  {
    // <NOT_ID_StartArithExpOrBexp> ::= <NOT_ID_StartArithExp> [ <BooleanOperator> <ArithExp> ]
    Form temp_Instruction_token ;
    string error ;
    // cout << "Start Check_NOT_IDStartArithExpOrBexp_Grammer\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {  
      if ( Check_NOT_ID_StartArithExp_Grammer() )
      {
        // cout << "Return to Check_NOT_IDStartArithExpOrBexp_Grammer1\n" ;  
      } // if
      else // ERROR Unexpected token
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;    
      } // else 
    
      temp_Instruction_token = mfirst.Peek_next_token() ;
      while ( temp_Instruction_token.mtype == "COMMAN" )
      {
        // cout << "CommanReader\n" ;
        mfirst.CommanReader() ;
        temp_Instruction_token = mfirst.Peek_next_token() ;
      } // while

      if ( temp_Instruction_token.mtoken != ";" )
      {
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if 

        if ( Check_BooleanOprator_Grammer() )
        {
          mcColumn++ ;
          if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
          {
            try
            {
              temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
              minstruction_buffer.push_back( temp_Instruction_token ) ; 
            } // try 
            catch( string error_type_1 ) 
            {
              // �qScanner�Ǩ�error �OUnrecognized token with first char 
              // cout << error_type_1 << endl ;
              error = error_type_1 ;
              Clear_buffer_and_line() ;
              throw error ;
              return false ;   
            } // catch
          } // if

          if ( Check_ArithExp_Grammer() )
          {
            // cout << "Return to Check_NOT_IDStartArithExpOrBexp_Grammer1\n" ;  
          } // if
          else 
          {
            error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
            throw error ;    
          } // else   
        } // if
        else 
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;  
        } // else 
      } // if
    } // if
    
    // cout << "Check_NOT_IDStartArithExpOrBexp_Grammer OK\n" ;
    return true ; // �N��NOT_IDStartArithExpOrBexp_Grammer���� 
  } // Check_NOT_IDStartArithExpOrBexp_Grammer()
  
  bool Check_NOT_ID_StartArithExp_Grammer() // �ˬdNOT_ID_StartArithExp��Grammer
  {
    // <ArithExp> ::= <Term> { '+' <Term> | '-' <Term> }
    Form temp_Instruction_token ;
    string error ;
    bool out_of_range = false ;
    // cout << "Start Check_NOT_ID_StartArithExp_Grammer\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      if ( Check_NOT_ID_StartTerm_Grammer() ) 
      {
        // cout << "Check_NOT_ID_StartArithExp_Grammer1\n" ; 
      } // if
      else // ERROR Unexpected token
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else 
    
      temp_Instruction_token = mfirst.Peek_next_token() ;
      while ( temp_Instruction_token.mtype == "COMMAN" )
      {
        // cout << "CommanReader\n" ;
        mfirst.CommanReader() ;
        temp_Instruction_token = mfirst.Peek_next_token() ;
      } // while

      // mcColumn++ ;
      while ( ( temp_Instruction_token.mtoken == "+" ) 
              || ( temp_Instruction_token.mtoken == "-" ) )
      {
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        mcColumn++ ;
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        if ( Check_Term_Grammer() ) 
        {  
          
          // cout << "Return to Check_NOT_ID_StartArithExp_Grammer2\n" ;
          // mcColumn++ ; 
        } // if
        else // ERROR Unexpected token
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;  
        } // else  
      
        temp_Instruction_token = mfirst.Peek_next_token() ;
        while ( temp_Instruction_token.mtype == "COMMAN" )
        {
          // cout << "CommanReader\n" ;
          mfirst.CommanReader() ;
          temp_Instruction_token = mfirst.Peek_next_token() ;
        } // while
      } // while 
      
    } // if 
      
    // cout << "Check_NOT_ID_StartArithExp_Grammer OK\n" ;
    return true ; // �N��Term����
  
  } // Check_NOT_ID_StartArithExp_Grammer()
  
  bool Check_NOT_ID_StartTerm_Grammer() // �ˬdNOT_ID_StartTerm 
  {
    Form temp_Instruction_token ; 
    string error ;
    // cout << "Start Check_Not_ID_STARTTerm_Grammer\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      if ( Check_NOT_ID_StartFactor_Grammer() ) 
      {
        // cout << "Return to Check_NOT_ID_StartTerm_Grammer1\n" ;
      } // if
      else // ERROR Unexpected token
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else 
    
      temp_Instruction_token = mfirst.Peek_next_token() ;
      while ( temp_Instruction_token.mtype == "COMMAN" )
      {
        // cout << "CommanReader\n" ;
        mfirst.CommanReader() ;
        temp_Instruction_token = mfirst.Peek_next_token() ;
      } // while
      // mcColumn++ ;
      while ( ( temp_Instruction_token.mtoken == "*" ) 
              ||  ( temp_Instruction_token.mtoken == "/" ) )
      {
        // �t�dŪ�J* / 
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
      
        mcColumn++ ;
      
        // �t�dŪ�JFactor
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
      
        if ( Check_Factor_Grammer() ) 
        {  
          // cout << "Return to Check_NOT_ID_StartTerm_Grammer2\n" ;
          // mcColumn++ ;
        } // if
        else // ERROR Unexpected token
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;  
        } // else  

        temp_Instruction_token = mfirst.Peek_next_token() ;
        while ( temp_Instruction_token.mtype == "COMMAN" )
        {
          // cout << "CommanReader\n" ;
          mfirst.CommanReader() ;
          temp_Instruction_token = mfirst.Peek_next_token() ;
        } // while 
      } // while 
      
    } // if
    
    // cout << "Check_NOT_ID_StartTerm_Grammer OK\n" ;
    return true ; // �N��Term����  
           
  } // Check_NOT_ID_StartTerm_Grammer()
  
  bool Check_NOT_ID_StartFactor_Grammer() // �ˬdNOT_ID_StartFactor
  {
    // <NOT_ID_StartFactor> ::= [ SIGN ] NUM | '(' <ArithExp> ')'
    // <NOT_ID_StartFactor> ::= [ SIGN ] NUM
    string error ;
    Form temp_Instruction_token ;
    // cout << "Start Check_NOT_ID_StartFactor_Grammer\n" ;
    if ( minstruction_buffer[mcColumn].mtoken == "+" 
         || minstruction_buffer[mcColumn].mtoken == "-" ) // �OSIGN 
    {
      mcColumn++ ;
      if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
      {
        try
        {
          temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
          minstruction_buffer.push_back( temp_Instruction_token ) ; 
        } // try 
        catch( string error_type_1 ) 
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          // cout << error_type_1 << endl ;
          error = error_type_1 ;
          Clear_buffer_and_line() ;
          throw error ;
          return false ;   
        } // catch
      } // if

      if ( minstruction_buffer[mcColumn].mtype == "INT" 
           || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �O���ݤU�@�ӬO�_INT FLOAT 
      {
        // cout << "It is a [Sign]NUM " << minstruction_buffer[mcColumn].mtoken 
        // << minstruction_buffer[mcColumn].mtoken << endl ;
        mcColumn++ ;
        return true ;    
      } // if
      else // ERROR Unexpected token 
      {  
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else    
    } // if
    else if ( minstruction_buffer[mcColumn].mtype == "INT" 
              || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �ONUM
    {
      // cout << "It is a NUM " << minstruction_buffer[mcColumn].mtoken << endl ;
      mcColumn++ ; 
      return true ;  
    } // else if    
    // <NOT_ID_StartFactor> ::= '(' <ArithExp> ')'
    else if ( minstruction_buffer[mcColumn].mtype == "LEFT_BRACKET" )
    {
      // cout << "It is a LEFT_BRACKET " << minstruction_buffer[mcColumn].mtoken << endl ;
      mcColumn++ ;
      if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
      {
        try
        {
          temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
          minstruction_buffer.push_back( temp_Instruction_token ) ; 
        } // try 
        catch( string error_type_1 ) 
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          // cout << error_type_1 << endl ;
          error = error_type_1 ;
          Clear_buffer_and_line() ;
          throw error ;
          return false ;   
        } // catch
      } // if
      
      if ( Check_ArithExp_Grammer() == true )
      {
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
    
        if ( minstruction_buffer[mcColumn].mtype == "RIGHT_BRACKET" )
        {
          mcColumn++ ;
          return true ;    
        } // if
        else 
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;    
        } // else   
      } // else if
      else // ERROR Unexpected token 
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else   
    } // if 
  
    
    return false ; // PL�n�[�� 
  } // Check_NOT_ID_StartFactor_Grammer()
  
  bool Check_BooleanOprator_Grammer() // �ˬdBooleanOprator
  {
    // <BooleanOprator> ::= '=' | '<>' | '>' | '<' | '>=' | '<=' 
    string error ;
    // cout << "Check_BooleanOprator_Grammer\n" ;
    if ( minstruction_buffer[mcColumn].mtoken == "=" )
    {
      return true ;  
    } // if
    else if ( minstruction_buffer[mcColumn].mtoken == ">" )
    {
      return true ;  
    } // else if
    else if ( minstruction_buffer[mcColumn].mtoken == ">=" )
    {
      return true ;  
    } // else if
    else if ( minstruction_buffer[mcColumn].mtoken == "<" )
    {
      return true ;  
    } // else if 
    else if ( minstruction_buffer[mcColumn].mtoken == "<=" )
    {
      return true ;  
    } // else if
    else if ( minstruction_buffer[mcColumn].mtoken == "<>" )
    {
      return true ;  
    } // else if
    else 
    {
      error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
      throw error ;     
    } // else   
  } // Check_BooleanOprator_Grammer()
  
  bool Check_ArithExp_Grammer() // �ˬdArithExp
  {
    // <ArithExp> ::= <Term> { '+' <Term> | '-' <Term> }
    Form temp_Instruction_token ;
    string error ;
    bool out_of_range = false ;
    // cout << "Start Check_ArithExp\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      if ( Check_Term_Grammer() ) 
      {
        // cout << "Return to Check_ArithExp1\n" ; 
      } // if
      else // ERROR Unexpected token
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else 
    
      temp_Instruction_token = mfirst.Peek_next_token() ;
      while ( temp_Instruction_token.mtype == "COMMAN" )
      {
        // cout << "CommanReader\n" ;
        mfirst.CommanReader() ;
        temp_Instruction_token = mfirst.Peek_next_token() ;
      } // while
      // mcColumn++ ;
      while ( ( temp_Instruction_token.mtoken == "+" ) 
              || ( temp_Instruction_token.mtoken == "-" ) )
      {
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        mcColumn++ ;
      
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        if ( Check_Term_Grammer() ) 
        {  
          
          // cout << "Return to Check_ArithExp2\n" ;
          // mcColumn++ ; 
        } // if
        else // ERROR Unexpected token
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;  
        } // else  
      
        temp_Instruction_token = mfirst.Peek_next_token() ;
        while ( temp_Instruction_token.mtype == "COMMAN" )
        {
          // cout << "CommanReader\n" ;
          mfirst.CommanReader() ;
          temp_Instruction_token = mfirst.Peek_next_token() ;
        } // while
      } // while 
      
    } // if 
      
    // cout << "Check_ArithExp OK\n" ;
    return true ; // �N��Term����
  
  } // Check_ArithExp_Grammer()
   
  bool Check_Term_Grammer() // �ˬdTerm��Grammer
  {
    // <Term> ::= <Factor> { '*' <Factor> | '/' <Factor> } 
    Form temp_Instruction_token ; 
    string error ;
    // cout << "Start Check_Term_Grammer\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      if ( Check_Factor_Grammer() ) 
      {
        // cout << "Return to Check_Term_Grammer1\n" ;
      } // if
      else // ERROR Unexpected token
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else 
    
      temp_Instruction_token = mfirst.Peek_next_token() ;
      while ( temp_Instruction_token.mtype == "COMMAN" )
      {
        // cout << "CommanReader\n" ;
        mfirst.CommanReader() ;
        temp_Instruction_token = mfirst.Peek_next_token() ;
      } // while
      // mcColumn++ ;
      while ( ( temp_Instruction_token.mtoken == "*" ) 
              ||  ( temp_Instruction_token.mtoken == "/" ) )
      {
        // �t�dŪ�J* / 
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
      
        mcColumn++ ;
      
        // �t�dŪ�JFactor
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if
      
        if ( Check_Factor_Grammer() ) 
        {  
          // cout << "Return to Check_Term_Grammer2\n" ;
          // mcColumn++ ;
        } // if
        else // ERROR Unexpected token
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;  
        } // else  

        temp_Instruction_token = mfirst.Peek_next_token() ;
        while ( temp_Instruction_token.mtype == "COMMAN" )
        {
          // cout << "CommanReader\n" ;
          mfirst.CommanReader() ;
          temp_Instruction_token = mfirst.Peek_next_token() ;
        } // while
      } // while 
      
    } // if
    
    // cout << "Check_Term_Grammer OK\n" ;
    return true ; // �N��Term����  
        
  } // Check_Term_Grammer()
  
  bool Check_Factor_Grammer() // �ˬdFactor��Grammer
  {
    // <Factor> ::= IDENT | [ SIGN ] NUM | '(' <ArithExp> ')'
    Form temp_Instruction_token ;
    bool is_ident_def = false ;
    string error ; 
    // cout << "Start Check_Factor_Grammer\n" ;
    // <Factor> ::= IDENT
    if ( minstruction_buffer[mcColumn].mtype == "IDENT" )
    {
      
      // cout << "It is IDENT " << minstruction_buffer[mcColumn].mtoken << endl ;
      mcColumn++ ;
      return true ;    
    } // if
    
    // <Factor> ::= [ SIGN ] NUM
    else if ( minstruction_buffer[mcColumn].mtoken == "+" 
              || minstruction_buffer[mcColumn].mtoken == "-" ) // �OSIGN 
    {
      mcColumn++ ;
      if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
      {
        try
        {
          temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
          minstruction_buffer.push_back( temp_Instruction_token ) ; 
        } // try 
        catch( string error_type_1 ) 
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          // cout << error_type_1 << endl ;
          error = error_type_1 ;
          Clear_buffer_and_line() ;
          throw error ;
          return false ;   
        } // catch
      } // if
    
      if ( minstruction_buffer[mcColumn].mtype == "INT" 
           || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �O���ݤU�@�ӬO�_INT FLOAT 
      {
        // cout << "It is a [Sign]NUM " 
        // << minstruction_buffer[mcColumn].mtoken << minstruction_buffer[mcColumn].mtoken << endl ; 
        mcColumn++ ;
        return true ;    
      } // if
      else // ERROR Unexpected token 
      {  
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;  
      } // else    
    } // else if
    else if ( minstruction_buffer[mcColumn].mtype == "INT" 
              || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �ONUM
    {
      // cout << "It is a NUM " << minstruction_buffer[mcColumn].mtoken << endl ;
      mcColumn++ ;
      return true ;  
    } // else if 
    
    // <Factor> ::= '(' <ArithExp> ')'
    
    else if ( minstruction_buffer[mcColumn].mtype == "LEFT_BRACKET" )
    {
      // cout << "It is a LEFT_BRACKET " << minstruction_buffer[mcColumn].mtoken << endl ;
      mcColumn++ ;
      if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
      {
        try
        {
          temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
          minstruction_buffer.push_back( temp_Instruction_token ) ; 
        } // try 
        catch( string error_type_1 ) 
        {
          // �qScanner�Ǩ�error �OUnrecognized token with first char 
          // cout << error_type_1 << endl ;
          error = error_type_1 ;
          Clear_buffer_and_line() ;
          throw error ;
          return false ;   
        } // catch
      } // if
    
      if ( Check_ArithExp_Grammer() == true )
      {
        
        if ( mcColumn == minstruction_buffer.size() ) // �N��minstruction_buffer�쩳,�nInput 
        {
          try
          {
            temp_Instruction_token = mfirst.Get_INPUT() ; // �qScanner�����X�@��Token
            minstruction_buffer.push_back( temp_Instruction_token ) ; 
          } // try 
          catch( string error_type_1 ) 
          {
            // �qScanner�Ǩ�error �OUnrecognized token with first char 
            // cout << error_type_1 << endl ;
            error = error_type_1 ;
            Clear_buffer_and_line() ;
            throw error ;
            return false ;   
          } // catch
        } // if

        if ( minstruction_buffer[mcColumn].mtype == "RIGHT_BRACKET" )
        {
          mcColumn++ ;
          return true ;    
        } // if
        else 
        {
          error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;    
        } // else   
      } // else if
      else 
      {
        error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
        throw error ;   
      } // else    
    } // else if 
    else // ERROR Unexpected token
    {
      error = "Unexpected token : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
      throw error ;    
    } // else 
    
    return false ; // PL�n�[��
  } // Check_Factor_Grammer()
  
  void Clear_buffer_and_line()
  {
    // �J��Error�n�N���e���minstruction_buffer�����B��Token�M��
    // �A�n��Error token��Input��M��
    minstruction_buffer.clear() ; // ���B��Token�M��
    mfirst.Clear_line() ;
    mcColumn = 0 ;  
  } // Clear_buffer_and_line()

  void Calculation() // �qminstruction_buffer�����ˬd��k,�íp�� (�֤�Function) Parser2
  {
    Form result ; // �x�s���G
    string error ;
    double ans = 0;
    double t1 ;
   
    stringstream tr_to_f ;
    // tr_to_f.setf(std::ios::fixed);
    tr_to_f.precision( 4 );
    result = Calculate_Command() ;
    // cout << "Result : " ;
    if ( result.mtype == "FLOAT" )
    {
      tr_to_f << result.mvalue ;
      tr_to_f >> ans ;
      t1 = ( int ) ( ans*10000 ) %1000%100%10;
      // cout << t1 << endl ;
      if ( t1 > 4 )
      {
        ans = ans + 0.0001 ;   
      } // if
      else 
      {
        ans = ans ;    
      } // else 

      printf( "%0.3f\n", ans ) ;
      // cout << fixed << setprecision(3) << ans << endl;  
    } // if
    else 
    {
      cout << result.mvalue << endl ;  
    } // else 

    minstruction_buffer.clear() ;  
  } // Calculation()
  
  bool Check_ID_Def( string token ) // ����ID�O�_�Q�w�q
  {
    int i = 0 ;
    bool def = false ;
    if ( mident_def_set.size() == 0 )
    {
      return false ;
    } // if
    else
    {
      while ( i < mident_def_set.size() )
      {
        if ( mident_def_set[i].mtoken == token )
        {
          def = true ;  
        } // if 

        i++ ;   
      } // while

      if ( def == true )
      {
        // cout << token << "is already def\n" ;
        return true ;  
      } // if 
      else 
      {
        string error ;
        error = "Undefined identifier : '" + token + "'" ;
        throw error ;  
      } // else 
    } // else    
  } // Check_ID_Def()
  
  void Add_ident_def_set( Form temp_id, Form operand ) // ��s�w�q��IDENT�[��ident_def_set�� 
  {
    int i = 0;
    int new_input_num = 0 ;
    bool same = false ;
    Form temp ;
    temp.mtoken = temp_id.mtoken ;
    temp.mtype = operand.mtype ;
    temp.mvalue = operand.mvalue ;
    // cout << "Start Add_ident_def_set\n" ;
    if ( mident_def_set.size() == 0 )
    {
      // cout << "Add new ident\n" ;
      mident_def_set.push_back( temp ) ;
      new_input_num = 0 ;  
    } // if
    else
    {
      while ( i < mident_def_set.size() )
      {
        if ( mident_def_set[i].mtoken == temp.mtoken )
        {
          mident_def_set[i].mtype = temp.mtype ;
          mident_def_set[i].mvalue = temp.mvalue  ;
          new_input_num = i ;
          same = true ;  
        } // if 

        i++ ;     
      } // while
    
      if ( same == false )
      {
        mident_def_set.push_back( temp ) ;
        new_input_num = mident_def_set.size() -1 ;  
      } // if  
    } // else  
    
    // cout << ident_def_set[new_input_num].token << " is add to ident_def_set\n" << "It's value is " 
    // << ident_def_set[new_input_num].value << endl ;    
  } // Add_ident_def_set()
 
  Form Get_ident_def( string token ) // �qident_def_set�����X���wForm  
  {
    int i = 0 ;
    // cout << "Start Get_ident_def\n" ;
    string error ;
    if ( Check_ID_Def( token ) )
    {
      while ( i < mident_def_set.size() )
      {
        if ( mident_def_set[i].mtoken == token )
        {
          return mident_def_set[i] ;   
        } // if
    
        i++ ;  
      } // while  
    } // if
    else 
    {
      error = "Undefined identifier : '" + minstruction_buffer[mcColumn].mtoken + "'" ;
      throw error ;     
    } // else 
    
    return mident_def_set[i] ; // PL�n�[��  
  } // Get_ident_def() 
  
  Form Calculate_Command() // �p��Command
  {
    Form temp_IDENT ;
    Form operand_1 ; // �B�⤸1 
    Form operand_2 ; // �B�⤸2 
    bool is_ident_def = false ;
    bool id_arith = false ;
    bool not_id = false ;
    bool is_IDlessArithExpOrBexp = false ;
    string error ;
    // cout << "Start Calculate_Command\n" ;
    // �ˬd IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'  
    if ( minstruction_buffer[mcColumn].mtype == "IDENT" )
    {
      // cout << "It is ID :" << minstruction_buffer[mcColumn].mtoken << endl ;
      temp_IDENT = minstruction_buffer[mcColumn] ;
      // mcColumn++ ; // �O�ocurrent_instruction_column_num++ �O�ѩI�s��/�W�h�t�d 
      //  ':=' <ArithExp>
      if ( minstruction_buffer[mcColumn+1].mtype == "ASSIGN" )
      {
        // cout << "It is ASSIGN :=\n" ;
        mcColumn++ ;
        mcColumn++ ;
        operand_1 = Calculate_ArithExp() ;
        // cout << "Return to Calculate_Command\n" ;
        mcColumn++ ;
      } // if
       
      // <IDlessArithExpOrBexp> 
      else if ( ( mcColumn < minstruction_buffer.size() ) )
      {
        // cout << "hi\n" ;
        if ( Check_ID_Def( minstruction_buffer[mcColumn].mtoken ) ) // �ˬd�O�_�Q�w�q�L 
        {
          // ���Q�w�q�L 
          is_IDlessArithExpOrBexp = true ;
          operand_1 = Get_ident_def( minstruction_buffer[mcColumn].mtoken ) ;
          operand_1 = Calculate_IDlessArithExpOrBexp( operand_1 ) ;
          // cout << "Return to Calculate_Command\n" ;
        } // if
        else
        {
          // cout << "hi2\n" ;
          error = "Undefined identifier : '" + 
          minstruction_buffer[mcColumn].mtoken + "'" ;
          throw error ;
        } // else 

      } // else if  

      // ';'  
      if ( minstruction_buffer[mcColumn].mtoken == ";" )
      {
        if ( is_IDlessArithExpOrBexp == true ) return operand_1 ;
        else 
        {
          // cout << "It is ;\n" ;
          Add_ident_def_set( temp_IDENT, operand_1 ) ; 
          // temp_IDENT�O IDENT:= ��IDENT, operand_1 �O�g�L�B�⪺���G
          return operand_1 ;
        } // else 
      } // if

    } // if 

    // �ˬd <NOT_IDStartArithExpOrBexp> ';'
    else
    {
      if ( ( mcColumn < minstruction_buffer.size() ) )
      {
        // cColumn++ ; 
        operand_1 = Calculate_NOT_IDStartArithExpOrBexp() ;
        // cout << "Return to Calculate_Command()\n" ;
        // ';'  
        if ( minstruction_buffer[mcColumn].mtoken == ";" )
        {
          return operand_1 ;
        } // if   
      } // if  

    } // else 

    return operand_1 ; 
  } // Calculate_Command()

  Form Calculate_IDlessArithExpOrBexp( Form ident ) // �p��IDlessArithExpOrBexp
  {
    // <IDlessArithExpOrBexp>  ::= { '+' <Term>  | '-' <Term>  | '*' <Factor> | '/' <Factor>  } 
    // [ <BooleanOperator> <ArithExp> ]
    string error ;
    Form operand_1 ; // �B�⤸1 
    Form operand_2 ; // �B�⤸1
    string op ; // �B�⦡
    string b_op ;
    operand_1 = ident ;
    // cout << "Start Calculate_IDlessArithExpOrBexp()\n" ;
    while ( ( mcColumn < minstruction_buffer.size()-1 ) 
            && ( ( minstruction_buffer[mcColumn+1].mtoken == "+" ) 
                 ||  ( minstruction_buffer[mcColumn+1].mtoken == "-" )  
                 || ( minstruction_buffer[mcColumn+1].mtoken == "*" ) 
                 ||  ( minstruction_buffer[mcColumn+1].mtoken == "/" ) ) )
    {
  
      if ( ( minstruction_buffer[mcColumn+1].mtoken == "+" ) 
           || ( minstruction_buffer[mcColumn+1].mtoken == "-" ) )
      {
        mcColumn++ ;
        // cout << "Start Calculate_IDlessArithExpOrBexp() + | - \n" ;
        op = minstruction_buffer[mcColumn].mtoken ; 
        mcColumn++ ;
        operand_2 = Calculate_Term() ; 
        // cout << "Return from Calculate_Term()1\n" ;
        operand_1 = Transform_to_calculable( operand_1, operand_2, op ) ;  
      } // if  
      else if ( ( minstruction_buffer[mcColumn+1].mtoken == "*" ) 
                ||  ( minstruction_buffer[mcColumn+1].mtoken == "/" ) ) 
      {
        mcColumn++ ;
        // cout << "Start Calculate_IDlessArithExpOrBexp() * | / \n" ;  
        op = minstruction_buffer[mcColumn].mtoken ; 
        mcColumn++ ;
        operand_2 = Calculate_Factor() ; 
        // cout << "Return from Calculate_Factor()1\n" ;
        operand_1 = Transform_to_calculable( operand_1, operand_2, op ) ;  
      } // else if 
    
      // mcColumn++ ; 
    } // while
    
    mcColumn++ ; 
    // cout << "Now cColumn is " << cColumn << endl ;
    // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
    if ( ( minstruction_buffer[mcColumn].mtype == "BOOL_OPERATOR" ) )
    {  
      // cout << "Have bool\n" ;
      b_op = minstruction_buffer[mcColumn].mtoken ;

      // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
      if ( mcColumn+1 < minstruction_buffer.size()-1 )
      {
        mcColumn++ ;  
        operand_2 = Calculate_ArithExp() ;
        operand_1 = Transform_to_calculable_boolean( operand_1, operand_2, b_op ) ;
      } // if
      
    } // if
    
    // cout << "Calculate_IDlessArithExpOrBexp() OK\n" ;
    return operand_1 ;
  } // Calculate_IDlessArithExpOrBexp()
  
  Form Calculate_NOT_IDStartArithExpOrBexp() // �p��NOT_IDStartArithExpOrBexp
  {
    // <NOT_ID_StartArithExpOrBexp> ::= <NOT_ID_StartArithExp> [ <BooleanOperator> <ArithExp> ]
    string error ;
    Form operand_1 ; // �B�⤸1 
    Form operand_2 ; // �B�⤸1
    string b_op ; // �B�⦡
    // cout << "Start Calculate_NOT_IDStartArithExpOrBexp\n" ;
    // cout << cColumn << " : " << first.Input_Instruction_Set[cLine][cColumn].token << endl ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {  
      operand_1 = Calculate_NOT_ID_StartArithExp() ;
      // cout << "Return to Calculate_NOT_IDStartArithExpOrBexp\n" ;
      // cout << "Now cColumn is " << cColumn << endl ;
      // cout << "Next Token is " << 
      // first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
      if ( minstruction_buffer[mcColumn+1].mtoken != ";" )
      {
        mcColumn++ ;  
        b_op = minstruction_buffer[mcColumn].mtoken ; 
        mcColumn++ ;
        operand_2 = Calculate_ArithExp() ;
        operand_1 = Transform_to_calculable_boolean( operand_1, operand_2, b_op ) ;
      } // if
      
      return operand_1 ;
    } // if
    
    return operand_1 ; // PL�n�[�� 
    // cout << "Calculate_NOT_IDStartArithExpOrBexp OK\n" ; // �N��NOT_IDStartArithExpOrBexp_Grammer���� 
  } // Calculate_NOT_IDStartArithExpOrBexp()
  
  Form Calculate_NOT_ID_StartArithExp() // �p��ArithExp
  {
    // <ArithExp> ::= <Term> { '+' <Term> | '-' <Term> }
    string error ;
    Form operand_1 ; // �B�⤸1 
    Form operand_2 ; // �B�⤸1
    // Form temp ;   // �O�@���@�ӼȮ��x�s�p�⵲�G��Form
    string op ; // �B�⦡ 
    // cout << "Start Calculate_ArithExp\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      operand_1 = Calculate_NOT_ID_StartTerm() ;   
      // cout << "Return from Calculate_Term()\n" ;
      // cout << "Now cColumn is " << cColumn << endl ;
      // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
    
      while ( ( minstruction_buffer[mcColumn+1].mtoken == "+" ) 
              || ( minstruction_buffer[mcColumn+1].mtoken == "-" ) )
      {
        mcColumn++ ;
        op = minstruction_buffer[mcColumn].mtoken ; 
        mcColumn++ ;
        operand_2 = Calculate_Term() ; 
        operand_1 = Transform_to_calculable( operand_1, operand_2, op ) ;
        // cout << "Now cColumn is " << cColumn << endl ;
        // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
      } // while 
      
    } // if 
      
    // cout << "Calculate_ArithExp OK\n" ;
    return operand_1 ; // �N��ArithExp����
  
  } // Calculate_NOT_ID_StartArithExp()
  
  Form Calculate_NOT_ID_StartTerm() // �p��Term
  {
    // <Term> ::= <Factor> { '*' <Factor> | '/' <Factor> }  
    string error ;
    Form operand_1 ; // �B�⤸1 
    Form operand_2 ; // �B�⤸1
    // Form temp ;   // �O�@���@�ӼȮ��x�s�p�⵲�G��Form
    string op ; // �B�⦡ 
    // cout << "Start Calculate_Term\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      operand_1 = Calculate_NOT_ID_StartFactor() ; 
      // cout << "Return from Calculate_Factor()\n" ;
      // cout << "Now cColumn is " << cColumn << endl ;
      // cout << "NExt Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
    
      while ( ( ( minstruction_buffer[mcColumn+1].mtoken == "*" ) 
                ||  ( minstruction_buffer[mcColumn+1].mtoken == "/" ) ) )
      {
        mcColumn++ ;
        op = minstruction_buffer[mcColumn].mtoken ; 
        mcColumn++ ;
        operand_2 = Calculate_Factor() ; 
        operand_1 = Transform_to_calculable( operand_1, operand_2, op ) ;

        // cout << "Now cColumn is " << cColumn << endl ;
        // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
      } // while 
      
    } // if
    
    // cout << "Calculate_Term OK\n" ;
    return operand_1 ; // �N��Term����  
        
  } // Calculate_NOT_ID_StartTerm()
  
  Form Calculate_NOT_ID_StartFactor() // �p��Factor
  {
    // <NOT_ID_StartFactor> ::= [ SIGN ] NUM | '(' <ArithExp> ')'
    Form temp ; // �O�@���@�ӼȮ��x�s�p�⵲�G��Form (�u��token(eg A+B),type,value)
    Form for_arith ; 
    string sign ;
    string error ; 
    stringstream tr ;
    double temp_value ;
    // cout << "Start Calculate_NOT_ID_StartFactor\n" ;
    // cout << "Now cColumn is " << cColumn << endl ;
    // cout << "NExt Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
    // <Factor> ::= IDENT

    // <Factor> ::= [ SIGN ] NUM
    if ( minstruction_buffer[mcColumn].mtoken == "+" 
         || minstruction_buffer[mcColumn].mtoken == "-" ) // �OSIGN 
    {
      minstruction_buffer[mcColumn].mtype = "SIGN" ;
      sign = minstruction_buffer[mcColumn].mtoken ;
      mcColumn++ ;
      if ( minstruction_buffer[mcColumn].mtype == "INT" 
           || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �O���ݤU�@�ӬO�_INT FLOAT 
      {
        // cout << "It is a [Sign]NUM " << minstruction_buffer[mcColumn-1].mtoken 
        // << minstruction_buffer[mcColumn].mtoken << endl ;
        if ( sign == "-" ) 
        {
          tr << minstruction_buffer[mcColumn].mvalue ;
          tr >> temp_value ;
          if ( temp_value == 0 )
          {
            temp.mtoken = minstruction_buffer[mcColumn].mtoken ;
            temp.mvalue = minstruction_buffer[mcColumn].mtoken ;
          } // if
          else
          {
            temp.mtoken = sign + minstruction_buffer[mcColumn].mtoken ;
            temp.mvalue = sign + minstruction_buffer[mcColumn].mtoken ;
          } // else   
        } // if
        else
        {
          temp.mtoken = minstruction_buffer[mcColumn].mtoken ;
          temp.mvalue = minstruction_buffer[mcColumn].mtoken ;
        } // else 
        
        if ( minstruction_buffer[mcColumn].mtype == "INT" )
        {
          temp.mtype = "INT" ;  
        } // if
        else 
        {
          temp.mtype = "FLOAT" ;  
        } // else  
       
        return temp ;
          
      } // if
      else // ERROR Unexpected token 
      {  
        error = "Unexpected token " + minstruction_buffer[mcColumn].mtoken ;
        throw error ;  
      } // else    
    } // if
    else if ( minstruction_buffer[mcColumn].mtype == "INT" 
              || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �ONUM
    {
      // cout << "It is a NUM " << minstruction_buffer[mcColumn].mtoken << endl ;
      temp.mtoken = minstruction_buffer[mcColumn].mtoken ;
      temp.mvalue = minstruction_buffer[mcColumn].mtoken ;
      if ( minstruction_buffer[mcColumn].mtype == "INT" )
      {
        temp.mtype = "INT" ;  
      } // if
      else 
      {
        temp.mtype = "FLOAT" ;  
      } // else  
       
      return temp ;
  
    } // else if 

    // <Factor> ::= '(' <ArithExp> ')'
    
    else if ( minstruction_buffer[mcColumn].mtype == "LEFT_BRACKET" )
    {
      // cout << "It is a LEFT_BRACKET " << first.Input_Instruction_Set[cLine][cColumn].token << endl ;
      mcColumn++ ;
      for_arith.mtoken = "(" ;
      temp = Calculate_ArithExp() ;
      if ( mcColumn < minstruction_buffer.size() )
      {
        mcColumn++ ;
        if ( minstruction_buffer[mcColumn].mtype == "RIGHT_BRACKET" )
        {
          for_arith.mtoken = for_arith.mtoken + temp.mtoken + ")";
          for_arith.mvalue = temp.mvalue ;
          for_arith.mtype = temp.mtype ;
          return for_arith ;    
        } // if
        else 
        {
          error = "Unexpected token :" + minstruction_buffer[mcColumn].mtoken ;
          throw error ;    
        } // else   
      } // if
      else // ERROR Unexpected token 
      {
        error = "Unexpected token :" + minstruction_buffer[mcColumn].mtoken ;
        throw error ;  
      } // else   
    } // else if 
    
    return temp ; // PL�n�[�� 
  } // Calculate_NOT_ID_StartFactor()
  
  Form Calculate_BooleanOprator() // �p��BooleanOprator
  {
    // <BooleanOprator> ::= '=' | '<>' | '>' | '<' | '>=' | '<=' 
    string error ;
    // cout << "Calculate_BooleanOprator\n" ;
    if ( minstruction_buffer[mcColumn].mtoken == "=" )
    {
      return minstruction_buffer[mcColumn] ;  
    } // if
    else if ( minstruction_buffer[mcColumn].mtoken == ">" )
    {
      return minstruction_buffer[mcColumn] ;  
    } // else if
    else if ( minstruction_buffer[mcColumn].mtoken == ">=" )
    {
      return minstruction_buffer[mcColumn] ;  
    } // else if
    else if ( minstruction_buffer[mcColumn].mtoken == "<" )
    {
      return minstruction_buffer[mcColumn] ;  
    } // else if
    else if ( minstruction_buffer[mcColumn].mtoken == "<=" )
    {
      return minstruction_buffer[mcColumn] ;  
    } // else if
    else if ( minstruction_buffer[mcColumn].mtoken == "<>" )
    {
      return minstruction_buffer[mcColumn] ;  
    } // else if  
    
    return minstruction_buffer[mcColumn] ; // PL�n�[�� 
  } // Calculate_BooleanOprator()
  
  Form Calculate_ArithExp() // �p��ArithExp
  {
    // <ArithExp> ::= <Term> { '+' <Term> | '-' <Term> }
    string error ;
    Form operand_1 ; // �B�⤸1 
    Form operand_2 ; // �B�⤸1
    // Form temp ;   // �O�@���@�ӼȮ��x�s�p�⵲�G��Form
    string op ; // �B�⦡ 
    // cout << "Start Calculate_ArithExp\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      operand_1 = Calculate_Term() ;   
      // cout << "Return from Calculate_Term()\n" ;
      // cout << "Now cColumn is " << cColumn << endl ;
      // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
    
      while ( ( minstruction_buffer[mcColumn+1].mtoken == "+" ) 
              || ( minstruction_buffer[mcColumn+1].mtoken == "-" ) )
      {
        mcColumn++ ;
        op = minstruction_buffer[mcColumn].mtoken ; 
        mcColumn++ ;
        operand_2 = Calculate_Term() ; 
        operand_1 = Transform_to_calculable( operand_1, operand_2, op ) ;
        // cout << "Now cColumn is " << cColumn << endl ;
        // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
      } // while 
      
    } // if 
      
    // cout << "Calculate_ArithExp OK\n" ;
    return operand_1 ; // �N��ArithExp����
  
  } // Calculate_ArithExp()
  
  Form Calculate_Term() // �p��Term
  {
    // <Term> ::= <Factor> { '*' <Factor> | '/' <Factor> }  
    string error ;
    Form operand_1 ; // �B�⤸1 
    Form operand_2 ; // �B�⤸1
    // Form temp ;   // �O�@���@�ӼȮ��x�s�p�⵲�G��Form
    string op ; // �B�⦡ 
    // cout << "Start Calculate_Term\n" ;
    if ( ( mcColumn < minstruction_buffer.size() ) )
    {
      operand_1 = Calculate_Factor() ; 
      // cout << "Return from Calculate_Factor()\n" ;
      // cout << "Now cColumn is " << cColumn << endl ;
      // cout << "NExt Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
    
      while ( ( ( minstruction_buffer[mcColumn+1].mtoken == "*" ) 
                ||  ( minstruction_buffer[mcColumn+1].mtoken == "/" ) ) )
      {
        mcColumn++ ;
        op = minstruction_buffer[mcColumn].mtoken ; 
        mcColumn++ ;
        operand_2 = Calculate_Factor() ; 
        operand_1 = Transform_to_calculable( operand_1, operand_2, op ) ;

        // cout << "Now cColumn is " << cColumn << endl ;
        // cout << "Next Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
      } // while 
      
    } // if
    
    // cout << "Calculate_Term OK\n" ;
    return operand_1 ; // �N��Term����  
        
  } // Calculate_Term()
  
  Form Calculate_Factor() // �p��Factor
  {
    // <Factor> ::= IDENT | [ SIGN ] NUM | '(' <ArithExp> ')'
    Form temp ; // �O�@���@�ӼȮ��x�s�p�⵲�G��Form (�u��token(eg A+B),type,value)
    Form for_arith ; 
    string sign ;
    string error ; 
    stringstream tr ;
    double temp_value ;
    // cout << "Start Calculate_Factor\n" ;
    // cout << "Now cColumn is " << cColumn << endl ;
    // cout << "NExt Token is " << first.Input_Instruction_Set[cLine][cColumn+1].token << endl ;
    // <Factor> ::= IDENT
    if ( minstruction_buffer[mcColumn].mtype == "IDENT" )
    {
      // cout << "It is IDENT " << minstruction_buffer[mcColumn].mtoken << endl ;
      return Get_ident_def( minstruction_buffer[mcColumn].mtoken ) ;
    // return first.Input_Instruction_Set[cLine][cColumn] ;    
    } // if
    
    // <Factor> ::= [ SIGN ] NUM
    else if ( minstruction_buffer[mcColumn].mtoken == "+" 
              || minstruction_buffer[mcColumn].mtoken == "-" ) // �OSIGN 
    {
      minstruction_buffer[mcColumn].mtype = "SIGN" ;
      sign = minstruction_buffer[mcColumn].mtoken ;
      mcColumn++ ;
      if ( minstruction_buffer[mcColumn].mtype == "INT" 
           || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �O���ݤU�@�ӬO�_INT FLOAT 
      {
        // cout << "It is a [Sign]NUM " << minstruction_buffer[mcColumn-1].mtoken 
        // << minstruction_buffer[mcColumn].mtoken << endl ;
        // temp.mtoken = sign + minstruction_buffer[mcColumn].mtoken ;
        // temp.mvalue = sign + minstruction_buffer[mcColumn].mtoken ;
        if ( sign == "-" ) 
        {
          tr << minstruction_buffer[mcColumn].mvalue ;
          tr >> temp_value ;
          if ( temp_value == 0 )
          {
            temp.mtoken = minstruction_buffer[mcColumn].mtoken ;
            temp.mvalue = minstruction_buffer[mcColumn].mtoken ;
          } // if
          else
          {
            temp.mtoken = sign + minstruction_buffer[mcColumn].mtoken ;
            temp.mvalue = sign + minstruction_buffer[mcColumn].mtoken ;
          } // else   
        } // if
        else 
        {
          temp.mtoken = minstruction_buffer[mcColumn].mtoken ;
          temp.mvalue = minstruction_buffer[mcColumn].mtoken ;  
        } // else

        if ( minstruction_buffer[mcColumn].mtype == "INT" )
        {
          temp.mtype = "INT" ;  
        } // if
        else 
        {
          temp.mtype = "FLOAT" ;  
        } // else  
       
        // cout << "he\n" ; 
        return temp ;
          
      } // if
      else // ERROR Unexpected token 
      {  
        error = "Unexpected token " + minstruction_buffer[mcColumn].mtoken ;
        throw error ;  
      } // else    
    } // else if
    else if ( minstruction_buffer[mcColumn].mtype == "INT" 
              || minstruction_buffer[mcColumn].mtype == "FLOAT" ) // �ONUM
    {
      // cout << "It is a NUM " << minstruction_buffer[mcColumn].mtoken << endl ;
      temp.mtoken = minstruction_buffer[mcColumn].mtoken ;
      temp.mvalue = minstruction_buffer[mcColumn].mtoken ;
      if ( minstruction_buffer[mcColumn].mtype == "INT" )
      {
        temp.mtype = "INT" ;  
      } // if
      else 
      {
        temp.mtype = "FLOAT" ;  
      } // else  
       
      return temp ;
  
    } // else if 

    // <Factor> ::= '(' <ArithExp> ')'
    
    else if ( minstruction_buffer[mcColumn].mtype == "LEFT_BRACKET" )
    {
      // cout << "It is a LEFT_BRACKET " << first.Input_Instruction_Set[cLine][cColumn].token << endl ;
      mcColumn++ ;
      for_arith.mtoken = "(" ;
      temp = Calculate_ArithExp() ;
      if ( mcColumn < minstruction_buffer.size() )
      {
        mcColumn++ ;
        if ( minstruction_buffer[mcColumn].mtype == "RIGHT_BRACKET" )
        {
          for_arith.mtoken = for_arith.mtoken + temp.mtoken + ")";
          for_arith.mvalue = temp.mvalue ;
          for_arith.mtype = temp.mtype ;
          return for_arith ;    
        } // if
        else 
        {
          error = "Unexpected token :" + minstruction_buffer[mcColumn].mtoken ;
          throw error ;    
        } // else   
      } // if
      else // ERROR Unexpected token 
      {
        error = "Unexpected token :" + minstruction_buffer[mcColumn].mtoken ;
        throw error ;  
      } // else   
    } // else if 
    
    return temp ; // PL�n�[�� 
  } // Calculate_Factor()
  
  Form Transform_to_calculable( Form operand1, Form operand2, string op ) 
  // �D�n�O��Form��string�ܬ�int|float,�A�ܬ�string 
  {
    Form temp ; // �^�ǥ�
    // string temp_token ;
    // stringstream tr ; // �૬�O�� 
    string error ;
    int temp_int = 0 ;
    float temp_double = 0;
    float temp_op_1 = 0 ;
    float temp_op_2 = 0 ;
    stringstream dtos ;
    dtos << fixed ;
    dtos.precision( 3 );
    stringstream tr1( operand1.mvalue ) ;
    tr1.precision( 100 );
    tr1 >> temp_op_1 ;
    stringstream tr2( operand2.mvalue ) ;
    tr2.precision( 100 );
    tr2 >> temp_op_2 ;  
    
    // cout << "YO\n" ; 
    // cout << operand1.mtoken << " " << temp_op_1 << endl ;
    // cout << operand2.mtoken << " " << temp_op_2 << endl ;
    // cout << "Yo2\n" ;
    if ( operand1.mtype == "FLOAT" )
    {
      if ( op == "+" )
      {  
        // temp_double = (operand1.value.c_str()) + atof(operand2.value.c_str()) ;
        temp_double = temp_op_1 + temp_op_2 ;  
      } // if
      else if ( op == "-" )
      {
        // temp_double = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
        temp_double = temp_op_1 - temp_op_2 ;    
      } // else if
      else if ( op == "*" )
      {
        // temp_double = atof(operand1.value.c_str()) * atof(operand2.value.c_str()) ;
        temp_double = temp_op_1 * temp_op_2 ;    
      } // else if
      else if ( op == "/" )
      {
        if ( temp_op_2 != 0 ) 
        {
          // temp_double = atof(operand1.value.c_str()) / atof(operand2.value.c_str()) ;
          temp_double = temp_op_1 / temp_op_2 ;    
        } // if 
        else 
        {
          error = "Error" ;
          throw error ;   
        } // else   
      } // else if  
    
      // cout << "Result : " << temp_double << endl ;
      temp.mtype = "FLOAT" ;
      temp.mtoken = operand1.mtoken + op + operand2.mtoken ;
      // printf( "%0.3f\n", temp_double );
      dtos << temp_double ;
      dtos >> temp.mvalue ;
      // cout << "Temp.value = " << temp.mvalue << endl ;
      // temp.value = temp_double ;  
    } // if
    else if ( operand2.mtype == "FLOAT" )
    {
      if ( op == "+" )
      {  
        // temp_double = (operand1.value.c_str()) + atof(operand2.value.c_str()) ;
        temp_double = temp_op_1 + temp_op_2 ;  
      } // if
      else if ( op == "-" )
      {
        // temp_double = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
        temp_double = temp_op_1 - temp_op_2 ;    
      } // else if
      else if ( op == "*" )
      {
        // temp_double = atof(operand1.value.c_str()) * atof(operand2.value.c_str()) ;
        temp_double = temp_op_1 * temp_op_2 ;    
      } // else if
      else if ( op == "/" )
      {
        if ( temp_op_2 != 0 ) 
        {
          // temp_double = atof(operand1.value.c_str()) / atof(operand2.value.c_str()) ;
          temp_double = temp_op_1 / temp_op_2 ;    
        } // if 
        else 
        {
          error = "Error" ;
          throw error ;   
        } // else   
      } // else if  
    
      // cout << "Result : " << temp_double << endl ;
      temp.mtype = "FLOAT" ;
      temp.mtoken = operand1.mtoken + op + operand2.mtoken ;
      // printf( "%0.3f\n", temp_double );
      dtos << temp_double ;
      dtos >> temp.mvalue ;
      // cout << "Temp.value = " << temp.mvalue << endl ;
      // temp.value = temp_double ;  
    } // else if
    else // �i�J�OINT 
    {
      // cout << "INT Cal\n" << op << endl; 
      if ( op == "+" )
      {  
        // temp_double = (operand1.value.c_str()) + atof(operand2.value.c_str()) ;
        temp_int = temp_op_1 + temp_op_2 ;  
      } // if
      else if ( op == "-" )
      {
        // temp_double = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
        temp_int = temp_op_1 - temp_op_2 ;    
      } // else if
      else if ( op == "*" )
      {
        // temp_double = atof(operand1.value.c_str()) * atof(operand2.value.c_str()) ;
        temp_int = temp_op_1 * temp_op_2 ;    
      } // else if
      else if ( op == "/" )
      {
        if ( temp_op_2 != 0 ) 
        {
          // temp_double = atof(operand1.value.c_str()) / atof(operand2.value.c_str()) ;
          // temp_double = temp_op_1 / temp_op_2 ;
          // if ( temp_double == ( int ) temp_double ) temp_int = temp_double ;
          temp_int = temp_op_1 / temp_op_2 ; 
        } // if 
        else 
        {
          error = "Error" ;
          throw error ;   
        } // else   
      } // else if  
    
      temp.mtoken = operand1.mtoken + op + operand2.mtoken ;
      // if ( temp_double == ( int ) temp_double )
      // {
      dtos << temp_int ;
      // } // if
      // else dtos << temp_double ;

      dtos >> temp.mvalue ; 
    } // else 
    
    // cout << "END : " << temp.mtoken << " " << temp.mvalue << endl ;  
    return temp ;
  } // Transform_to_calculable()
  
  Form Transform_to_calculable_boolean( Form operand1, Form operand2, string b_op ) 
  // �D�n�O�@boolean�p��,�A�ܬ�string
  {
    Form temp ;
    string temp_token ;
    float temp_value ;
    float temp_op_1 ;
    float temp_op_2 ;
    stringstream dtos ;
    dtos << fixed ;
    stringstream tr1( operand1.mvalue ) ;
    tr1 << fixed ;
    tr1.precision( 100 );
    tr1 >> temp_op_1 ;
    // cout << "op1" << temp_op_1 ;
    stringstream tr2( operand2.mvalue ) ;
    tr2 << fixed ;
    tr2.precision( 100 );
    tr2 >> temp_op_2 ;
    // cout << "op2" << temp_op_2 ;  
    // cout << "IN\n" ;
    // cout << operand1.mvalue << " " << operand2.mvalue << endl ;
    // cout << temp_op_1 << " " << temp_op_2 << endl ;   
    if ( b_op == "=" )
    {
      temp_value = temp_op_1 - temp_op_2 ; 
      if ( temp_value < 0 ) 
      {
        temp_value = -temp_value ;  
      } // if
      // cout << "Temp value : " << temp_value << endl ;
      // temp_value = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
      if ( temp_value < 0.0001 ) // �~�t
      {
        // cout << "hi1 \n" ;
        // if ( temp_op_1 == temp_op_2 )
        if ( temp_value < 0.0001 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // else 
      } // if
      else
      {
        if ( temp_value == 0 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // else   
      } // else 
    } // if
    else if ( b_op == "<>" )
    {
      temp_value = temp_op_1 - temp_op_2 ;
      if ( temp_value < 0 ) 
      {
        temp_value = -temp_value ;   
      } // if
      // temp_value = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
      if ( temp_value <= 0.0001 ) // �~�t�� 
      {
        // if ( temp_op_1 != temp_op_2 )
        if ( temp_value != 0 )
        {
          // cout << "yo\n" ;
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // else 
      } // if
      else
      {
        if ( temp_value == 0 )
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // if
        else 
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // else   
      } // else 
    } // else if
    else if ( b_op == ">=" )
    {
      temp_value = temp_op_1 - temp_op_2 ;
      if ( temp_value < 0 ) 
      {
        temp_value = -temp_value ;   
      } // if
      // cout << temp_value << endl ;
      // temp_value = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
      if ( temp_value <= 0.000101 ) // �~�t�� 
      {
        // cout << "hi3 \n" ;
        if ( temp_value >= 0 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          // cout << "yo\n" ;
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // else   
      } // if
      else
      {
        if ( temp_op_1 >= temp_op_2 ) 
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // else  
      } // else   
    } // else if
    else if ( b_op == ">" )
    {
        // temp_value = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
      temp_value = temp_op_1 - temp_op_2 ;
      if ( temp_value < 0 ) 
      {
        temp_value = -temp_value ;   
      } // if
      
      if ( temp_value > 0.0001 ) // �~�t�� 
      {
        // cout << "hi4 \n" ;
        if ( temp_op_1 > temp_op_2 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // else
      } // if
      else
      {
        if ( temp_op_1  > temp_op_2 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // else  
      } // else     
    } // else if 
    else if ( b_op == "<=" )
    {
        // temp_value = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
      temp_value = temp_op_1 - temp_op_2 ;
      if ( temp_value < 0 ) 
      {
        temp_value = -temp_value ;  
      } // if
      
      if ( temp_value <= 0.000101 ) // �~�t�� 
      {
        // cout << "hi5 \n" ;
        if ( temp_op_1 <= temp_op_2 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // else  
      } // if
      else
      {
        if ( temp_op_1 <= temp_op_2 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // else  
      } // else  
    } // else if 
    else if ( b_op == "<" )
    {
        // temp_value = atof(operand1.value.c_str()) - atof(operand2.value.c_str()) ;
      temp_value = temp_op_1 - temp_op_2 ;
      if ( temp_value < 0 ) 
      {
        temp_value = -temp_value ;   
      } // if
      
      if ( temp_value > 0.0001 ) // �~�t�� 
      {
        // cout << "hi6 \n" ;
        if ( temp_op_1 < temp_op_2 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // else
      } // if
      else
      {
        if ( temp_op_1 < temp_op_2 )
        {
          temp.mvalue = "true" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;  
        } // if
        else 
        {
          temp.mvalue = "false" ;
          temp.mtype = "Boolean" ;
          temp.mtoken = operand1.mtoken + b_op + operand2.mtoken ;   
        } // else  
      } // else  
    } // else if 
    
    return temp ;   
  } // Transform_to_calculable_boolean()
} ; // Parser

int main()
{
  char file_num ;
  scanf( "%c", &file_num ) ;
  Parser start ;
  start.Start_Scanner() ;
  return 0 ;  
} // main()
