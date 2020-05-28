# CSS Stylesheet Organizer

Keep your external stylesheet files clean with this C++ script. 

## Installation

 1. Clone repo into directory.
 2. Using the console, write "make run". This will begin compiling the script for you.
 3. You will be prompted to give the path to your directory containing the stylesheets.
 4. Done, let the script do its work!
 
 ## Usage
 ### Before running
```css
:root {
    --text-color: #fff;
    --background-color: #1f2935;

    --primary-color: #455363;
}

h3 {
    color :green;
}

.date-time {
    font-size: 12px;
    color: var(--text-color);
}

.data-number {
    color: #fff;
    font-size: 32px;
}
.page-title {
    color: var(--text-color);
}
```
### After running
```css
.data-number {
    color: #fff;
    font-size: 32px;
}

.date-time {
    font-size: 12px;
    color: var(--text-color);
}

.page-title {
    color: var(--text-color);
}

:root {
    --background-color: #1f2935;
    --text-color: #fff;
    --primary-color: #455363;
}

h3 {
    color : green;
}
```

 
 ## Contributing
 
 Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
 
 Please make sure to update documentation as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
