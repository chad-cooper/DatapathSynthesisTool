LIBRARY ieee;
  use ieee.std_logic_1164.ALL;

-- The input is a bit_vector of N bits. Output is NAND of these bits
entity gen_NAND_bit is
  generic (width : INTEGER := 4);
  port ( input : std_logic_vector(width-1 downto 0);
	 output : out std_logic);
end gen_NAND_bit;

architecture Behavior of gen_NAND_bit is
begin
   P0 : process(input)
	  variable  result : std_logic;
        begin
	  result := '0';
	  L1 :
	   for n in width-1 downto 0 loop
	     if input(n)='0' then
		result := '1';
		exit L1;
	     end if;
	  end loop L1;

	  output <= result;
   end process P0;

end Behavior;


