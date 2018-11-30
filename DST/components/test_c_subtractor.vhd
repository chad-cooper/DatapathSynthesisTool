
use Std.Textio.all;
library IEEE;

library worklib;

use ieee.std_logic_1164.ALL;

entity test_c_subtractor
is end;

architecture test_c_subtractor of test_c_subtractor is

component c_subtractor
    generic(width: integer := 4);
    port(input1, input2: in std_logic_vector((width-1) downto 0);
	 output :   out std_logic_vector(width downto 0));
end component;

for all : c_subtractor use entity work.c_subtractor(behavior);

signal input1 : std_logic_vector(3 downto 0);
signal input2 : std_logic_vector(3 downto 0);
signal out1 : 	std_logic_vector(4 downto 0);
    
begin

subtractor4 : c_subtractor generic map(4)
	port map( input1, input2, out1);

test_process : process 

	begin

        -- 0 - 1 = -1 
	input1 <= "0000" ;
	input2 <= "0001" ;
	wait for 10 ns;

	-- +7 - 5 = +2 
	input1 <= "0111" ;
	input2 <= "0101" ;
	wait for 10 ns;

	-- +5 - 7 = -2
	input1 <= "0101" ;
	input2 <= "0111" ;
	wait for 10 ns;

	-- -8 -8 = -16
	input1 <= "1000" ;
	input2 <= "1000" ;
	wait for 10 ns;

	-- -7 +7 = 0
	input1 <= "1001" ;
	input2 <= "0111" ;
	wait for 10 ns;
	
        wait;
end process test_process ;
end test_c_subtractor;

---------------------------------------------------------------------------
---------------------------------------------------------------------------


