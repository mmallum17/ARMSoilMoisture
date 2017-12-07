function rand() {
    return Math.random();
}

Plotly.plot('graph', [{
    y: [1,2,3].map(rand),
    mode: 'lines',
    line: {color: '#80CAF6'}
}]);

var cnt = 0;

/*var xhttp = new XMLHttpRequest();
xhttp.open("GET", "http://18.221.30.192/data", true);
xhttp.onreadystatechange = function(){
  if(xhttp.readyState == 4 && xhttp.status == 200){
      console.log(xhttp.responseText);
  }
};
xhttp.send();*/


/*var interval = setInterval(function() {

    Plotly.extendTraces('graph', {
        y: [[rand()]]
    }, [0]);

    if(cnt === 100) clearInterval(interval);
}, 300);*/