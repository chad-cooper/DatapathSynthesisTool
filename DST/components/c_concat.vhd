LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity c_concat is
  generic (width1, width2 : integer := 4);
  port (input1 : in std_logic_vector((width1 - 1) downto 0);
        input2 : in std_logic_vector((width2 - 1) downto 0);
        output : out std_logic_vector((width1+width2-1) downto 0));
end c_concat;

architecture behavior of c_concat is
begin
  P0 : process(input1,input2)
  begin
    for1 : FOR i IN 0 to (width2-1) LOOP
      output(i) <= input2(i);
    END LOOP for1;
    for2 : FOR i IN 0 to (width1-1) LOOP
      output(width2+i) <= input1(i);
    END LOOP for2;
  end process P0;
end behavior;
  

