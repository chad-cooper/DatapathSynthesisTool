Library ieee;
use ieee.std_logic_1164.all;

entity c_bus is
    generic (width : integer;
	     no_of_inputs : integer); 
	port(	
	input : in Std_logic_vector (((width*no_of_inputs) - 1) downto 0);
	BUS_SELECT : in Std_logic_vector ((no_of_inputs - 1) downto 0);
	output : out Std_logic_vector ((width - 1) downto 0)); 
end c_bus; 

architecture behavior of c_bus is
begin  -- behavior
    
    P1: process(bus_select, input)
        variable control_val    : integer := 0;
	variable drive : Boolean := FALSE;
      begin
	  for i in 0 to (no_of_inputs - 1) loop
	      if (bus_select(i) = '1') then
		  output <= input(((i+1)*width-1) downto (i)*width);
		  drive := TRUE;	
	      end if;	
	  end loop;

	  if(drive = FALSE) then
	     for i in 0 to (width - 1) loop
		  output(i) <= 'Z';
	     end loop;
          end if;	
      end process P1;
end behavior;
