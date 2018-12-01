LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
  use ieee.std_logic_arith.all;
--  use ieee.std_logic_signed.all;
  
LIBRARY WORK;
  use WORK.ALL;

entity c_multiplier is
  generic(width: integer := 4);
  port(input1: std_logic_vector((width-1) downto 0);
       input2: std_logic_vector((width-1) downto 0);
       output: out std_logic_vector(width-1 downto 0);
end c_multiplier;

architecture behavior of c_multiplier is
   function bits_to_int (input : std_logic_vector )return INTEGER is
     variable ret_val : INTEGER := 0;
   begin
     for i in input'RANGE loop
       if input(i) = '1' then
         ret_val := 2**i + ret_val;
       end if;
     end loop;
     return ret_val;
   end bits_to_int;
begin
  P0 : process(input1,input2)
      	 variable l_val, r_val, value : Integer;
	 variable result : std_logic_Vector((2*width - 1) downto 0);
       begin
	  l_val := bits_to_int(Input1);
	  r_val := bits_to_int(Input2);
	  value := l_val * r_val;
          for i in 0 to 2*width - 1 loop
	     if (value rem 2) = 1 then
	        result(i) := '1';
	     else
	        result(i) := '0';
	     end if;
	     value := value / 2;
          end loop;
	  output <= result(width-1 downto 0);
       end process P0;
	 
--         variable result: std_logic_vector((2*width) - 1 downto 0);
--         variable result1 : integer := 0;
--       begin
--          result1 := input1 * input2;
--          output <= result((2*width)-2 downto 0);
--       end process P0;
end behavior;


