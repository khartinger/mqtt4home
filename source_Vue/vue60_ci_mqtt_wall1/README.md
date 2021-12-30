Last modified: 2021-12-27 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></td><td>
<h1>How to create wall elements</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

## Target
This Vue project shows how to create wall elements that match the CI grid.   
The following image shows examples of these wall elements:   

![wall_elements](./images/vue60_ci_mqtt_wall1_view1.png "wall_elements")   
_Fig. 1: wall elements_   

The type of the graphic is specified as a number, which is composed as the sum of the following values:   

![wall_type](./images/vue60_ci_mqtt_wall1_type1.png "wall_type")   
_Fig. 2: Calculation of the wall type_   

_Examples:_   
` 1 + 2 = 3` : wall like an L   
` 1 + 4 = 5` : Horizontal line --   
` 2 + 8 = 10` : Vertical line |   
`16 + 32 = 48` : Wall like a V   
`1 + 2 + 8 = 11` : Vertical wall with a horizontal extension on the right |-   
`2 + 4 + 8 = 14` : Vertical wall with a horizontal extension on the left -|   
etc.   
