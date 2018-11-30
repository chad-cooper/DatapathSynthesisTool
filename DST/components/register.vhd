
LIBRARY IEEE;
use ieee.std_logic_1164.ALL;
  
--LIBRARY WORK;
--use WORK.ALL;

entity c_register is
  generic (width : integer := 4);
  port (input : in std_logic_vector((width-1) downto 0);
        WR: in std_logic;
        clear : in std_logic;
        clock : in std_logic;
        output : out std_logic_vector((width -1) downto 0));
end c_register;

architecture behavior of c_register is
begin
  P0 : process(clock, input, WR, clear)
         variable out_var : std_logic_vector((width - 1) downto 0);
         variable Interim_Val : std_logic_vector((width-1) downto 0);
       begin
	   
	  Interim_Val := input;

          if (clear = '1') then
	      for I in width - 1 downto 0 loop
		  out_var(I) := '0';
	      end loop;
              output <= out_var;
	  end if;

	  if (clear = '0') then
           if ((clock = '1') and (clock'event = TRUE) and (WR = '1')) then
             output <= Interim_Val;
	   end if;
          end if;

       end process P0;
end behavior;
