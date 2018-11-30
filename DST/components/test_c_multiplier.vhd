use work.all;

Library ieee; 
  use ieee.std_logic_1164.ALL;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_signed.all;

entity test_c_multiplier is 
end ;

architecture test_c_multiplier of test_c_multiplier is 

component c_multiplier 
  generic(width: integer := 4);
  port(input1: std_logic_vector((width-1) downto 0);
       input2: std_logic_vector((width-1) downto 0);
       output: out std_logic_vector((width*2)-2 downto 0));
end component;

for all : c_multiplier use entity work.c_multiplier(Behavior); 

signal input1 : std_logic_vector((4-1) downto 0) := "0000";
signal input2 : std_logic_vector((4-1) downto 0) := "0000";
signal output : std_logic_vector((4*2)-2 downto 0) := "0000000";

begin

  mult1 : c_multiplier
	generic map(4)
	port map( input1, input2, output);

  simulate : process
  begin
      
	input1 <= "0001"; 
	input2 <= "0010";

        wait for 10 ns;

	input1 <= "1001"; 
	input2 <= "1010";

	wait for 10 ns;

	input1 <= "1001"; 
	input2 <= "0010"; 
	
	wait for 10 ns;

	wait;
	
  end process simulate;			

end test_c_multiplier;

 
