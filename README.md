## Instructions on how my game works:

1. In main window, you can choose 'Menu' to get help or view Scoreboard.

2. By clicking 'start' you will get options on how to start the game.

3. If the chosen starting way is fill manually, it will lead you to grid window where
you have to select the cells you want to start with and press 'Done' when finished.

If you choose fill from file and toggle default , if it can't read default.txt it will show a message box.

4. A spin box 'Game size' allows you to choose the size of the grid before launching.
I restricted it from 2 - 10 for my window. And 6 is the default.

I changed the filling methods (fill_from_input and fill_randomly) in gameboard to accept a parameter called size.
So I didn't have to change sthe constant variable SIZE. size is initialized both in gridwindow and mainwindow.

'Launch' will load the actual game afterwards, taking you to grid window.

5. On grid window, you see disabled grid buttons at start, which are the inputs. I designed it in such a way
    you are only allowed to manipulate the remaining positions during the gametime.

6. On grid window, you can pause the game, which will halt the time and disable the all the grid buttons.
   I designed it in such a way that you can stop time to think of your next move.

   The pause button can be used again as 'resume' to continue playing.

7. On grid window, reset button will reset all button you have chosen once the game started, and start the
    time from scratch. Whereas 'Quit' closes the window.

8. When pressing a button on the grid, there are three options poping. If you chose 'Empty' it will reset the
    value at that position. The others are to fill as '0' or '1'.

9. For wrong moves, my implementation bars erroneous addition. Instead, it indicates you have made a wrong move by a red light.
   Then the button is reset to empty. Also a wrong move will deduct your posible earning points by 10.
   The more erroneous moves you make, the lesser your point will be.

10. The game only ends if the board is fully filled with '0's and '1's. Once done, a text browser will pop up with alternating
   background color and shows your points and the elapsed time.

11. Points are calculated based on the time elapsed. I took a numerator of 1 hour (3600 seconds) as a base, then you divide the elasped
   time in seconds to it. It would be easy to finish fast with small grids so you also multiply that by your grid size for fairness.
   And if there were point deductions, it will compute all of them together.

12. Finally, by clicking 'Quit' you will return to main window. In top left corner Menu->Scoreboard you find a table which stores scores
    of completed games and other information with it like the date, time elapsed, and the game size used.
    'Reset' button will remove all the existing scores, while 'About' will explain how points are calculated.

    There is also a 'Help' menu which will explain game rules.
