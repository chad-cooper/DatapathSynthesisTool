LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_signed.all;

entity c_divider is
  generic(width: integer := 4;const: integer := 16);
  port(input: std_logic_vector((width-1) downto 0);
       output: out std_logic_vector((width - 1) downto 0));
end c_divider;

architecture behavior of c_divider is

begin
  P0 : process(input)
         variable l_val, r_val, value: Integer;
         VARIABLE flag: Integer := 0;
         variable result: std_logic_vector(width - 1 downto 0);
       begin
          if ((const > 2**(width-2)) or (const = 0)) then
            
               for i in 0 to (width-1) loop
                 result(i) := '0';
               end loop;
             end if;

          if ((const > 0) and (const <= 2**(width-2))) then
            l_val := CONV_INTEGER(input);
            value := l_val / const;
            result := CONV_STD_LOGIC_VECTOR(value,width);
          end if;
          output <= result;
       end process P0;
end behavior;


