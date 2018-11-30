LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
  use ieee.std_logic_arith.all;
LIBRARY WORK;
  use WORK.ALL;


entity constant_reg is
     generic (width : integer := 4;
              const : integer := 7);
     port ( output : out std_logic_vector((width -1) downto 0));
end constant_reg;

architecture behavior of constant_reg is
begin
  P0 : process
       begin
          output <= CONV_STD_LOGIC_VECTOR(const,width);
          wait;
       end process P0;
end behavior;

