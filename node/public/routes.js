import React from 'react'
import {
	BrowserRouter as Router,
	Routes, 
	Link
} from 'react-router-dom'

const CurrencyRouter = () => (
	<Router>
		<div>
      <ul>
        <li><Link to="/">Home</Link></li>
        <li><Link to="/dashboard">Dashboard</Link></li>
        <li><Link to="/arbitrage">Arbitrage</Link></li>
      </ul>

      <hr/>

      <Route exact path="/" component={Home}/>
      <Route path="/dashboard" component={About}/>
      <Route path="/arbitrage" component={Topics}/>
    </div>
	</Router>
)

export CurrencyRouter