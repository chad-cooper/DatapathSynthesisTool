use Std.Textio.all;
library IEEE;
use ieee.std_logic_1164.ALL;

entity test_divider is 
end;

architecture test_divider of test_divider is

component c_divider
   generic(width : INTEGER ; const :INTEGER
           );
   port(Input: in std_logic_vector((width - 1) downto 0);
        Output : out std_logic_vector((width - 1) downto 0));
end component;

for all : c_divider use entity WORK.c_divider(behavior);

signal Input: std_logic_vector(3 downto 0);
signal Output : std_logic_vector(3 downto 0);
file S_IN : TEXT is  out "c_divider_beh.out";

begin

   divider_1 : c_divider 
             generic map(4,2)
             port map(Input, Output);

   test_process : process
   begin

	Input <= "1010";
	wait for 50 ns;

	Input <= "0101";
	wait for 50 ns;

        Input <= "0110";
	wait for 50 ns;

        Input <= "0000";
	wait for 50 ns;

        Input <= "0000";
	wait for 50 ns;

        Input <= "0101";
	wait for 50 ns;

        Input <= "1111";
	wait for 50 ns;

        Input <= "1001";
	wait for 50 ns;

	Input <= "1110";
	wait for 50 ns;

        wait;

end process test_process;


end test_divider;
      

