function draw_board() {
	var board_size = 19;
	var board = document.getElementById('board');

	board.innerHTML += '<table class="board" '
		+ 'cellpadding=0 cellspacing=0 border=0>';

	for (var y = 0; y < board_size; y++) {
		board.innerHTML += "<tr>";
		for (var x = 0; x < board_size; x++) {
			var img = "";
			
			if (y == 0 && x==0)
				img = "0.gif";
			else if (x == (board_size-1)/2 && y == (board_size-1)/2)
				img = "9.gif";
			else if (y == 0 && x < board_size-1)
				img = "1.gif";
			else if (y == 0 && x < board_size)
				img = "2.gif";
			else if (y < board_size-1 && x == 0)
				img = "3.gif";
			else if (y < board_size-1 && x < board_size-1)
				img = "4.gif";
			else if (y < board_size-1 && x < board_size)
				img = "5.gif";
			else if (y == board_size-1 && x == 0)
				img = "6.gif";
			else if (y == board_size-1 && x < board_size-1)
				img = "7.gif";
			else if (y == board_size-1 && x == board_size-1)
				img = "8.gif";


			board.innerHTML += "<td><input type='image' border='0' name='" 
				+ y + "" + x  
				+ "' src='images/" + img +"' height=30 width=30></td>";
		}

		board.innerHTML += "</tr>";
	} 

	board.innerHTML += "</table>";
	

}