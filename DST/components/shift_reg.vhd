LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity shift_reg is
  generic (width : integer := 4);
  port (input : in std_logic_vector((width-1) downto 0);
        control : in std_logic_vector(1 downto 0);
        clear : in std_logic;
        clock : in std_logic;
        output : out std_logic_vector((width -1) downto 0));
end shift_reg;

architecture behavior of shift_reg is
begin
  P0 : process(clock,control,clear,input)
         variable out_var : std_logic_vector((width - 1) downto 0);
         variable Interim_Val : std_logic_vector((width-1) downto 0);
       begin
          for I in width - 1 downto 0 loop
             out_var(I) := '0';
          end loop;

          if (clear = '1') then
             output <= out_var;
             Interim_Val := out_var;
          elsif (clock = '1' and not clock'STABLE and control = "01") then
             output <= input;
          end if;
          
          if (clock = '1' and not clock'STABLE and control = "10") then
             -- right shift
            
             for I in 0 to width-2 loop
                Interim_Val(i) := Interim_Val(i+1);
             end loop;
             Interim_Val(width-1) := '0';
             output <= Interim_Val;
          end if;
                    
          if (clock = '1' and not clock'STABLE and control = "11") then
             -- left shift
             for I in width-1 downto 1 loop
                Interim_Val(i) := Interim_Val(i-1);
             end loop;
             Interim_Val(0) := '0';
             output <= Interim_Val;
          end if;
         end process P0;
end behavior;

