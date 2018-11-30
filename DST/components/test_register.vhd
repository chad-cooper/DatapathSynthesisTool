use Std.Textio.all;
library IEEE;
use ieee.std_logic_1164.ALL;

entity test_c_register is end;

architecture test_c_register of test_c_register is

component c_register
  generic (width : INTEGER := 4);
  port (input : in std_logic_vector((width - 1) downto 0);
	WR: in std_logic;
        clear : in std_logic;
        clock : in std_logic;
        output : out std_logic_vector((width -1) downto 0));
end component;

for all : c_register use entity work.c_register(behavior);

signal INPUT : std_logic_vector(3 downto 0);
signal CLEAR : std_logic ;
SIGNAL clock : std_logic;
signal OUTPUT : std_logic_vector(3 downto 0);
signal WR : std_logic;

begin

creg1 : c_register generic map(4)
	port map( input, WR, clear, clock , output);

	test_process : process 

	begin

	----------- check clear ----------------	

         
		CLEAR <= '1';
		WR <= '0' ;
                clock <= '1';
		
                input <= "1101";
                wait for 10 ns;
		
                clock <= '0';
                wait for 10 ns;
         
		CLEAR <= '0';
	        WR <= '1';
		
                input <= "0111";
		
                clock <= '1';
                wait for 10 ns;
		
                clock <= '0';
                wait for 10 ns;

		input <= "0101";
		
                clock <= '1';
                wait for 10 ns;
		
                clock <= '0';
                wait for 10 ns;

		WR <= '0';

		input <= "0011";
		
                clock <= '1';
                wait for 10 ns;
		
                clock <= '0';
                wait for 10 ns;

		clock <= '1';
                wait for 10 ns;
		
                clock <= '0';
                wait for 10 ns;
                
                wait;


end process test_process ;

end test_c_register;

---------------------------------------------------------------------------
---------------------------------------------------------------------------


