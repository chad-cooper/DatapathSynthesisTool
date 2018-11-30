
use Std.Textio.all;
library IEEE;

library worklib;

use ieee.std_logic_1164.ALL;

entity test_c_and
is end;

architecture test_c_and of test_c_and is

component c_and
  generic (width : integer := 1);
  port (input1 : std_logic_vector((width -1)  downto 0);
        input2 : std_logic_vector((width -1)  downto 0);
        output : out std_logic_vector((width -1) downto 0));
end component;

for all : c_and use entity worklib.c_and(behavior);

signal input1 : std_logic_vector(3 downto 0);
signal input2 : std_logic_vector(3 downto 0);
signal out1 : 	std_logic_vector(3 downto 0);
    
begin

and4 : c_and generic map(4)
	port map( input1, input2, out1);

test_process : process 

	begin

	input1 <= "0001" ;
	input2 <= "0001" ;
	wait for 10 ns;

	input1 <= "1111" ;
	input2 <= "0001" ;
	wait for 10 ns;

	input1 <= "1111" ;
	input2 <= "0101" ;
	wait for 10 ns;

        wait;


end process test_process ;

end test_c_and;

---------------------------------------------------------------------------
---------------------------------------------------------------------------


