LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity c_multiplexer is
        generic(width   : integer := 4;
        no_of_inputs    : integer:= 2;
        select_size     : integer:=1);
        port (
        input           : in std_logic_vector
                           (((width * no_of_inputs)-1)  downto 0);
        mux_select      : in std_logic_vector ((select_size-1) downto 0);
        output          : out std_logic_vector ((width-1) downto 0));
end c_multiplexer;

architecture behavior of c_multiplexer is
begin
P1 : process(mux_select,input)
        variable control_val    : integer := 0;
        variable base_val       : integer := 1;
      begin
        base_val := 1;
        control_val := 0;
        for i in 0 to (select_size-1) loop
          if (mux_select(i) = '1') then
            control_val := control_val + base_val;
          end if;
          base_val := base_val * 2;
        end loop;
        output <= input(((control_val+1)*width-1) downto (control_val)*width);
   end process P1;

end behavior;
