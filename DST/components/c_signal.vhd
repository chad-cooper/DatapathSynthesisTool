LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity c_signal is
        generic (width : integer := 4);
        port ( input : in std_logic_vector((width-1) downto 0);
               store, update, clear, clock : in std_logic;
               output : out std_logic_vector((width+1) downto 0));
end c_signal;

ARCHITECTURE behavior of c_signal is
  begin
  P0 : process(clock,store,update,input,clear)
         variable In_latch : std_logic_Vector((width-1) downto 0);
         variable Out_latch : std_logic_Vector((width-1) downto 0);
         variable out_var : std_logic_Vector((width - 1) downto 0);
         variable sig_stable : std_logic;
         variable sig_quiet : std_logic;
       begin

          if (clock = '1' and not clock'STABLE and store = '1') then
             In_latch := Input;
             sig_quiet := '0';
          end if;

          if (clock = '1' and not clock'STABLE and update = '1') then
             sig_stable := '1';
             L1 : for I in width - 1 downto 0 loop
                  if not (In_latch(I) = Out_latch(I)) then
                    sig_stable := '0';
                    exit L1;
                  end if;
             end loop L1;
             Out_latch := In_latch;
             output((width-1) downto 0) <= Out_latch;
             output(width) <= sig_stable;
             output(width+1) <= sig_quiet;
             sig_quiet := '1';
          end if;

          if (clear = '1') then
             L2 : for I in (width - 1) downto 0 loop
                  In_latch(I) := '0';
                  Out_latch(I) := '0';
             end loop L2;
             output((width-1) downto 0) <= Out_latch;
             output(width) <= '1';
             output(width+1) <= '1';
             sig_quiet := '1';
          end if;

       end process P0;
end behavior;
