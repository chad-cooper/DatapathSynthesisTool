Library ieee;
use ieee.std_logic_1164.all;

entity tri_state_buf is
generic (width : integer := 4);
port (input : std_logic_vector((width-1) downto 0);
      enable : std_logic;
      output : out std_logic_vector((width-1) downto 0));
end tri_state_buf;

architecture behavior of tri_state_buf is
begin

  process(input,enable)
  begin
    if (enable = '1') 
    then 
       output <= input;
    else
       output <= (others => 'Z');
    end if;
  end process;
end behavior;
