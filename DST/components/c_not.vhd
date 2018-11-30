LIBRARY ieee;
  use ieee.std_logic_1164.ALL;

entity c_not is
  generic (width : integer := 1);
  port (input : std_logic_vector((width-1)  downto 0);
        output : out std_logic_vector((width-1) downto 0));
end  c_not;

architecture behavior of c_not is
begin
	 P0 : process(input)
          variable  result : std_logic_vector(width-1 downto 0);
        begin
          outer :
           for n in width-1 downto 0 loop
             result(n) := NOT  input(n);
          end loop outer;
          output <= result;
   end process P0;

end behavior;
