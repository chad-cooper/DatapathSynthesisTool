LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity c_divider is
  generic (width : integer := 4;
           const: integer := 16);
  port (input1,input2 : in std_logic_vector((width-1)  downto 0);
        output : out std_logic_vector((width-1) downto 0));
end c_divider;

architecture behavior of c_divider is
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

   function twocomp_int_to_bin (INPUT,wid : INTEGER)return STD_LOGIC_VECTOR is
      
        variable TEMP_A, TEMP_B: INTEGER := 0;
        variable OUTPUT: STD_LOGIC_VECTOR((wid-1) downto 0);
        variable comp_input, abs_val: integer;
        begin
        if (input < 0) then
           abs_val := 0 - input;
           comp_input := (2**(wid-1)) - abs_val;
           assert(comp_input >= 0);
        else
           comp_input := input;
        end if;
        TEMP_A := comp_input;
        for I in (wid - 2) downto 0 loop
                TEMP_B := TEMP_A/(2**I);
                TEMP_A := TEMP_A rem (2**I);
                if (TEMP_B = 1) then
                        OUTPUT(I) := '1';
                else
                        OUTPUT(I) := '0';
                end if;
        end loop;
        if (input < 0) then
           OUTPUT(wid-1) := '1';
        else
           OUTPUT(wid-1) := '0';
        end if;
        return OUTPUT;
        end twocomp_int_to_bin;

  

begin
  P0 : process(input1,input2)
         variable l_val, r_val, value : Integer;
         variable result : std_logic_vector((width - 1) downto 0);
       begin
          l_val := twocomp_bits_to_int(input1);
          r_val := twocomp_bits_to_int(input2);
          if not (r_val = 0)
          then
          value := l_val / r_val;
          result := twocomp_int_to_bin(value,width);
          output <= result;
          end if;
       end process P0;
end behavior;











