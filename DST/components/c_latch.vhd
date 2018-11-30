LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity c_latch is
 generic (width : integer := 4);
      port (input : in std_logic_vector((width-1) downto 0);
        enable : in std_logic; clear : in std_logic; clock : in std_logic;
        output : out std_logic_vector((width-1) downto 0));
end c_latch;

architecture behavior of c_latch is
begin
  P0 : process(clock,clear,input,enable)
         variable out_var : std_logic_vector((width - 1) downto 0);
       begin
          for I in width - 1 downto 0 loop
             out_var(I) := '0';
          end loop;

          if (clear = '1') then
             output <= out_var;
                            
          elsif  (clock = '1' and not clock'STABLE and enable = '1') then
             output <= input;
          end if;
          
       end process P0;
end behavior;
