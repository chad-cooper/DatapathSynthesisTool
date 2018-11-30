use Std.Textio.all;
library IEEE;
use ieee.std_logic_1164.ALL;

entity test_shiftreg is end;

architecture test_shiftreg of test_shiftreg is

component shift_reg
	  generic (width : INTEGER := 4);
  port (input : in std_logic_vector((width - 1) downto 0);
        control:in std_logic_vector(1 downto 0);
        Clear :in std_logic; clock	: in std_logic;
        Output : out std_logic_vector((width - 1) downto 0));
end component;

for all : shift_reg use entity work.shift_reg(behavior);

signal INPUT : std_logic_vector(3 downto 0);
signal control : std_logic_vector(1 downto 0); 
signal CLEAR : std_logic ;
SIGNAL clock : std_logic;
signal OUTPUT : std_logic_vector(3 downto 0);

begin

Latch1 : shift_reg generic map(4)
		port map( input, control, clear, clock , output);

	test_process : process 

	begin

	----------- check clear ----------------	

         
		CLEAR <= '1';
		control <= "01" ;
                clock <= '1';
                control <= "01";
                input <= "1101";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;
                

         
		CLEAR <= '1';
		control <= "01" ;
                clock <= '1';
                control <= "01";
                input <= "0111";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;
                
   
         ----------- check Control ---------------
		
                CLEAR <= '0';
                control <= "01";
                clock <= '1';
    	        input <= "0100";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                control <= "10";
                clock <= '1';
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                control <= "01";
		wait for 10 ns;
                clock <= '1';
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                control <= "11";
                clock <= '1';
    	        wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                CLEAR <= '1';

                control <= "11";
                clock <= '1';
    	        wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                wait;


end process test_process ;

end test_shiftreg;

---------------------------------------------------------------------------
---------------------------------------------------------------------------


