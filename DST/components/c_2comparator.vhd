LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity c_comparator is
   generic (width : integer := 16 --width
        );
   port (input1 : in std_logic_vector((width -1) downto 0);
         input2 : in std_logic_vector((width-1) downto 0);
         output : out std_logic_vector(2 downto 0)  --lt, gt, eq
        );
end c_comparator;

ARCHITECTURE behavior  OF c_comparator IS
  
function twocomp_bits_to_int (input : std_logic_vector )return INTEGER is
   variable ret_val : INTEGER := 0;
   begin
   for i in input'RANGE loop
    if (i < input'HIGH) then
     if (input(input'HIGH) = '0') then
      if input(i) = '1' then
        ret_val := 2**i + ret_val;
      end if;
     else
      if input(i) = '0' then
        ret_val := 2**i + ret_val;
      end if;
     end if;
    end if;
   end loop;
   if (input(input'HIGH) = '1') then
     ret_val := ret_val + 1;
     ret_val := 0 - ret_val;
   end if;
   return ret_val;
  end twocomp_bits_to_int;
  
BEGIN  --  behavior  

  P0 : process(input1,input2)
         variable result : std_logic_vector(2 downto 0);
         variable inp1, inp2 : INTEGER;
       begin
         result := "000";
         inp1 := twocomp_bits_to_int(input1);
         inp2 := twocomp_bits_to_int(input2);
         if (inp1 = inp2) then
            result(0) := '1';
         end if;
         if (inp1 > inp2) then
            result(1) := '1';
         end if;
         if (inp1 < inp2) then
            result(2) := '1';
         end if;
         output <= result;
       end process P0;
 
END behavior ;
