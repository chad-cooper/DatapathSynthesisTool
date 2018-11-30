use Std.Textio.all;
LIBRARY ieee;
  use ieee.std_logic_1164.ALL;
LIBRARY WORK;
  use WORK.ALL;

entity test_Signal is end;

architecture test_Signal of test_Signal is

component C_Signal 
  generic (width : INTEGER := 4);
  port (Input : in std_logic_Vector((width - 1) downto 0);
        Store, Update, Clear, clock : in std_logic;
        Output : out std_logic_Vector((width + 1) downto 0));
end component;
for all : C_Signal use entity Work.C_Signal(Behavior);

signal INPUT : std_logic_vector(3 downto 0) ; 
signal OUTPUT : std_logic_vector(5 downto 0) ; 
signal CLEAR : std_logic ; 

signal clock : std_logic ; 
signal STORE : std_logic ; 
signal UPDATE : std_logic ; 


begin

Signal1 : C_Signal generic map (4)
		port map( Clear => CLEAR , clock => clock, input(3) => INPUT(3),input(2) => INPUT(2),input(1) => INPUT(1),input(0) => INPUT(0), output(5)=>output(5),output(4)=>output(4),output(3) => OUTPUT(3),output(2) => OUTPUT(2),output(1) => OUTPUT(1),output(0) => OUTPUT(0), Store => STORE, Update => UPDATE);

	test_process : process 

	begin
                clear <= '0';
                clock <= '1';
                store <= '1';
                update <= '0';
                input <= "1001";
                WAIT FOR 50 ns;
                clock <= '0';
                WAIT FOR 50 ns;

                clear <= '0';
		clock <= '1';
                store <= '0';
                update <= '1';
                WAIT FOR 50 ns;
                clock <= '0';
                WAIT FOR 50 ns;
		
                clear <= '0';
		clock <= '1';
	        store <= '0';
                update <= '1';
                WAIT FOR 50 ns;
                clock <= '0';
                WAIT FOR 50 ns;

                clear <= '0';
                clock <= '1';
                store <= '1';
                update <= '1';
                input <= "1111";
                WAIT FOR 50 ns;
                clock <= '0';
                WAIT FOR 50 ns;

                clear <= '1';
                clock <= '1';
                store <= '1';
                update <= '1';
                input <= "1111";
                WAIT FOR 50 ns;
                clock <= '0';
                WAIT FOR 50 ns;
                

                
		wait;
	------------ end checking -------------- 

end process test_process ;


end test_Signal;

---------------------------------------------------------------------------
---------------------------------------------------------------------------


