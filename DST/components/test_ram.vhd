library IEEE;
use IEEE.std_logic_1164.all;
use Std.Textio.all;
use IEEE.std_logic_textio.all;

entity test_ram is
end test_ram;

architecture test_ram of test_ram is

component Ram 
   generic (width : INTEGER := 16;
	 ram_select : INTEGER := 4);	
   port (input1 : in std_logic_Vector((width - 1) downto 0);
         input2 : in std_logic_Vector((ram_select - 1) downto 0);
         Wr, Rd, CLOCK : in std_logic;
         output : out std_logic_Vector(width - 1 downto 0)) ;
end component;

for all : Ram use entity WORK.Ram(behavior);

signal input1 :  std_logic_Vector(3 downto 0);
signal input2 :  std_logic_Vector(3 downto 0);
signal Wr, Rd, Clk : std_logic;
signal output :  std_logic_Vector(3 downto 0);

begin
UUT : Ram 
      generic map(4,4)

      port map(input1, input2, Wr, rd, Clk, output);

test_process : process
begin

Clk <= '1';
wr <= '1';
rd <= '0';
input1 <= "1111";
input2 <= "0000";
wait for 100 ns;
Clk <= '0';
wait for 100 ns;

Clk <= '1';
wr <= '1';
rd <= '0';
input1 <= "1110";
input2 <= "0001";
wait for 100 ns;
Clk <= '0';
wait for 100 ns;

Clk <= '1';
wr <= '1';
rd <= '0';
input1 <= "1101";
input2 <= "0010";
wait for 100 ns;
Clk <= '0';
wait for 100 ns;

Clk <= '1';
wr <= '0';
rd <= '1';
input2 <= "0000";
wait for 100 ns;
Clk <= '0';
wait for 100 ns;

Clk <= '1';
wr <= '0';
rd <= '1';
input2 <= "0001";
wait for 100 ns;
Clk <= '0';
wait for 100 ns;

Clk <= '1';
wr <= '0';
rd <= '1';
input2 <= "0010";
wait for 100 ns;
Clk <= '0';
wait for 100 ns;


wait;

end process;

monitor_process : process
variable outline : line;
file outfile : text is out "ram.out";
begin
 write(outline, input1);
 write(outline, string'(" : "));
 write(outline, input2);
 write(outline, string'(" : "));
 write(outline, rd);
 write(outline, string'(" : "));
 write(outline, wr);
 write(outline, string'(" :   "));
 write(outline, output);
 writeline(outfile, outline);
 wait until not rd'quiet;

end process;
end test_ram;

