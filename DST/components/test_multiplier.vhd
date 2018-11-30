use Std.Textio.all;
library IEEE;
use ieee.std_logic_1164.ALL;

entity test_multiplier is 
end;

architecture test_multiplier of test_multiplier is

component c_multiplier
   generic(width : INTEGER 
           );
   port(Input1, Input2 : in std_logic_vector((width - 1) downto 0);
        Output : out std_logic_vector((2*width - 2) downto 0));
end component;

for all : c_multiplier use entity WORK.c_multiplier(behavior);

signal Input1, Input2 : std_logic_vector(3 downto 0);
signal Output : std_logic_vector(6 downto 0);
file S_IN : TEXT is  out "c_multiplier_beh.out";

begin

   multiplier_1 : c_multiplier 
             generic map(4)
             port map(Input1, Input2, Output);

   test_process : process
   begin

	Input1 <= "1010";
	Input2 <= "0001";
	wait for 50 ns;

	Input1 <= "0101";
	Input2 <= "0011";
	wait for 50 ns;

        Input1 <= "0110";
        Input2 <= "0100";
	wait for 50 ns;

        Input1 <= "0000";
        Input2 <= "1000";
	wait for 50 ns;

        Input1 <= "0000";
        Input2 <= "0000";
	wait for 50 ns;

        Input1 <= "0101";
        Input2 <= "0111";
	wait for 50 ns;

        Input1 <= "1111";
        Input2 <= "1111";
	wait for 50 ns;

        Input1 <= "1001";
        Input2 <= "0010";
	wait for 50 ns;

	Input1 <= "1110";
	Input2 <= "0010";
	wait for 50 ns;

        wait;

end process test_process;


end test_multiplier;
      

