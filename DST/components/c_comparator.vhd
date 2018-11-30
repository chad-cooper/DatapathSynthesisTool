LIBRARY ieee;
  use ieee.std_logic_1164.ALL;

entity c_comparator is
   generic (width : integer := 16 --width
        );
   port (input1 : in std_logic_vector((width -1) downto 0);
         input2 : in std_logic_vector((width-1) downto 0);
         output : out std_logic_vector(2 downto 0)  --lt, gt, eq
        );
end c_comparator;

ARCHITECTURE behavior  OF c_comparator IS
  
BEGIN  --  behavior  

  P0 : process(input1,input2)
         variable result : std_logic_vector(2 downto 0);
       begin
         result := "000";
         if (input1 = input2) then
            result(0) := '1';
         elsif (input1 > input2) then
            result(1) := '1';
         elsif (input1 < input2) then
            result(2) := '1';
         end if;
         output <= result;
       end process P0;
 
END behavior ;
