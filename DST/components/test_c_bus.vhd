use Std.Textio.all;
library IEEE;
use ieee.std_logic_1164.ALL;

entity test_c_bus is end;

architecture test_c_bus of test_c_bus is

component c_bus
    generic (width : integer;
	     no_of_inputs : integer); 
	port(	
	input : in Std_logic_vector (((width*no_of_inputs) - 1) downto 0);
	BUS_SELECT : in Std_logic_vector ((no_of_inputs - 1) downto 0);
	output : out Std_logic_vector ((width - 1) downto 0)); 
end component; 

for all : c_bus use entity work.c_bus(behavior);

signal INPUT : std_logic_vector(15 downto 0);
signal OUTPUT : std_logic_vector(3 downto 0);
signal BUS_SELECT : std_logic_vector(3 downto 0);

begin

bus1 : c_bus generic map(4, 4)
	port map( input, bus_select, output);

	test_process : process 

	begin
	    bus_select <= "0001";

	    input <= "0011001000010000";
	    wait for 10 ns;

    	    bus_select <= "0010";
    	    wait for 10 ns;

      	    bus_select <= "0100";
    	    wait for 10 ns;

       	    bus_select <= "1000";
    	    wait for 10 ns;

       	    bus_select <= "0000";
    	    wait for 10 ns;
	
	    wait;
	end process test_process ;

end test_c_bus;

---------------------------------------------------------------------------
---------------------------------------------------------------------------


