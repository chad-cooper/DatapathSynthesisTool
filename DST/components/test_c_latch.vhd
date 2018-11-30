use Std.Textio.all;
library IEEE;
use ieee.std_logic_1164.ALL;

entity test_latch is end;

architecture test_latch of test_latch is

component c_Latch 
	  generic (width : INTEGER := 4);
  port (input : in std_logic_vector((width - 1) downto 0);
        Enable:in std_logic; Clear :in std_logic; clock	: in std_logic;
        Output : out std_logic_vector((width - 1) downto 0));
end component;

for all : c_latch use entity work.c_latch(behavior);

signal INPUT : std_logic_vector(3 downto 0);
signal ENABLE : std_logic ;
signal CLEAR : std_logic ;
SIGNAL clock : std_logic;
signal OUTPUT : std_logic_vector(3 downto 0);

begin

Latch1 : c_latch generic map(4)
		port map( input, enable, clear, clock , output);

	test_process : process 

	begin

	----------- check clear ----------------	

         
		CLEAR <= '1';
		Enable <= '1' ;
                clock <= '1';
                Enable <= '1';
                input <= "1101";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;
                

         
		CLEAR <= '1';
		Enable <= '1' ;
                clock <= '1';
                Enable <= '1';
                input <= "0111";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;
                
   
         ----------- check Control ---------------
		
                CLEAR <= '0';
                Enable <= '1';
                clock <= '1';
    	        input <= "0100";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                clock <= '1';
    	        input <= "1110";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                clock <= '1';
    	        input <= "0001";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;

                Enable <= '0';
                clock <= '1';
    	        input <= "0101";
                wait for 10 ns;
                clock <= '0';
                wait for 10 ns;
 
                wait;


end process test_process ;

end test_latch;

---------------------------------------------------------------------------
---------------------------------------------------------------------------


