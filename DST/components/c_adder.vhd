
LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;


entity c_adder is
  generic(width: integer := 4);
  port(input1, input2: in std_logic_vector((width-1) downto 0);
       output :   out std_logic_vector(width-1 downto 0));
end c_adder;

architecture behavior of c_adder is
begin
  P0 : process(input1,input2)
      VARIABLE  carry : std_logic := '0';
      VARIABLE  overflow : std_logic := '0';
      VARIABLE  temp     : std_logic_vector(width downto 0);
  begin
    for i in 0 to width - 1 loop
       if input1(i) = '0' and input2(i) = '0' and carry = '0' then
          temp(i) := '0';
          carry := '0';
       elsif input1(i) = '0' and input2(i) = '0' and carry = '1' then
          temp(i) := '1';
          carry := '0';
       elsif input1(i) = '0' and input2(i) = '1' and carry = '0' then
          temp(i) := '1';
          carry := '0';
       elsif input1(i) = '0' and input2(i) = '1' and carry = '1' then
          temp(i) := '0';
          carry := '1';
       elsif input1(i) = '1' and input2(i) = '0' and carry = '0' then
          temp(i) := '1';
          carry := '0';
       elsif input1(i) = '1' and input2(i) = '0' and carry = '1' then
          temp(i) := '0';
          carry := '1';
       elsif input1(i) = '1' and input2(i) = '1' and carry = '0' then
          temp(i) := '0';
          carry := '1';
       elsif input1(i) = '1' and input2(i) = '1' and carry = '1' then
          temp(i) := '1';
          carry := '1';
       end if;
     end loop;
     temp(width) := carry;
     carry := '0';
     output <= temp(width-1 downto 0);

     --  Overflow detection
    
     IF ((input1(width-1) = '0') AND  (input2(width-1) = '0')  AND  (temp(width) ='1')) THEN
       overflow := '1';
     ELSIF ((input1(width-1) = '1') AND  (input2(width-1) = '1')  AND  (temp(width) ='0')) THEN
        overflow := '1';
     ELSIF ((input1(width-1) = '0') AND  (input2(width-1) = '1')  AND  (temp(width) ='1') ) THEN
       overflow := '1';
     ELSIF ((input1(width-1) = '1') AND  (input2(width-1) = '0')  AND  (temp(width) ='0')) THEN
       overflow := '1';
     END IF;

     end process;

end behavior;
